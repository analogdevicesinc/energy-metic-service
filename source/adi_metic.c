/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file	    	adi_metic.c
 * @brief       API definitions to communicate with Metrology IC for read, write registers.
 * @{
 */
/*============= I N C L U D E S =============*/

#include "adi_metic.h"
#include "ade9178_default.h"
#include "adi_metic_private.h"
#include "adi_metic_status.h"
#include <stdint.h>
#include <string.h>

/*============= P R O T O T Y P E S =============*/

/**
 * @brief Checks integrity error
 * @param[in] pInfo	  - pointer to Metrology Service Data
 */
static ADI_METIC_STATUS CheckIntegrityError(ADI_METIC_INFO *pInfo);

/**
 * @brief Checks startup error
 * @param[in] pInfo	  - pointer to Metrology Service Data
 */
static ADI_METIC_STATUS CheckStartupError(ADI_METIC_INFO *pInfo);
static void MeticCallBack(ADI_METIC_MASTER_EVENT_TYPE eventType, void *pData);

/*=============  C O D E  =============*/

ADI_METIC_STATUS adi_metic_Create(ADI_METIC_HANDLE *phAde, void *pStateMemory,
                                  uint32_t stateMemorySize)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    uint32_t reqSize = sizeof(ADI_METIC_INFO);

    /* Check the given pointers before we set their contents */
    if ((phAde == (void *)NULL) || (pStateMemory == (void *)NULL))
    {
        return ADI_METIC_STATUS_NULL_PTR;
    }
    if (status == ADI_METIC_STATUS_SUCCESS)
    {
        /* Set handle to NULL in case of error */
        *phAde = (ADI_METIC_HANDLE)NULL;
        if (stateMemorySize < reqSize)
        {
            status = ADI_METIC_STATUS_INSUFFICIENT_STATE_MEMORY;
        }
        else
        {
            *phAde = pStateMemory;
        }
    }
    return status;
}

ADI_METIC_STATUS adi_metic_Close(ADI_METIC_HANDLE hAde)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        if (pInfo->meticConfig.pfClose != NULL)
        {
            pInfo->meticConfig.pfClose(pInfo->meticConfig.hUser);
        }
    }

    return status;
}

ADI_METIC_STATUS adi_metic_Init(ADI_METIC_HANDLE hAde, ADI_METIC_CONFIG *pConfig)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if ((hAde == NULL) || (pConfig == NULL))
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo->meticConfig = *pConfig;
        pInfo->wfsData.isWfsRxComplete = 1;
        pInfo->wfsData.config.offsetCount = ADI_METIC_WFS_OFFSET_COUNT;
    }

    return status;
}

ADI_METIC_STATUS adi_metic_SetConfig(ADI_METIC_HANDLE hAde, ADI_METIC_CONFIG *pConfig)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if ((hAde == NULL) || (pConfig == NULL))
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo->meticConfig = *pConfig;
    }
    return status;
}

ADI_METIC_STATUS adi_metic_ReadRegister(ADI_METIC_HANDLE hAde, uint8_t device, uint16_t addr,
                                        uint32_t numRegisters, int32_t *pValue)
{

    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    int32_t data = 0;
    uint16_t cmd = ADI_METIC_CMD_READ_REGISTER;
    uint32_t numBytesReceived = 0;
    int32_t *pTempResponse;

    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pTempResponse = &pInfo->responseBuffer[0];
        if (numRegisters > ADI_METIC_MAX_NUM_REGISTERS)
        {
            status = ADI_METIC_STATUS_INVALID_NUM_REGISTERS;
        }
        else
        {
            status = adi_metic_SendCmdGetResponse(hAde, device, addr, cmd, numRegisters, &data,
                                                  pTempResponse, &numBytesReceived);
            memcpy(pValue, pTempResponse, numBytesReceived);
        }
    }

    return status;
}

ADI_METIC_STATUS adi_metic_WriteRegister(ADI_METIC_HANDLE hAde, uint8_t device, uint16_t addr,
                                         int32_t *pValue)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    int32_t *pTempResponse;
    uint16_t cmd = ADI_METIC_CMD_WRITE_REGISTER;
    uint32_t numReceivedBytes = 0;
    /* ADE9178 doesn't support multiple register writes */
    uint16_t numRegisters = 1;

    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pTempResponse = &pInfo->responseBuffer[0];
        status = adi_metic_SendCmdGetResponse(hAde, device, addr, cmd, numRegisters, pValue,
                                              pTempResponse, &numReceivedBytes);

        memcpy(pValue, pTempResponse, numReceivedBytes);
    }

    return status;
}

ADI_METIC_STATUS adi_metic_CheckStartupStatus(ADI_METIC_HANDLE hAde)
{

    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    int32_t status0;
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        status = CheckIntegrityError(pInfo);
        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            status = CheckStartupError(pInfo);
        }

        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            if (pInfo->irq0Ready == 1)
            {
                pInfo->irq0Ready = 0;
                status = adi_metic_ReadRegister(pInfo, 0, ADE9178_REG_STATUS0, 1, &status0);
                if (status == ADI_METIC_STATUS_SUCCESS)
                {
                    status = ADI_METIC_STATUS_STARTUP_PENDING;
                    if ((status0 & ADE9178_BITM_STATUS0_RSTDONE) != 0)
                    {
                        status = ADI_METIC_STATUS_SUCCESS;
                        status0 = ADE9178_BITM_STATUS0_RSTDONE;
                        pInfo->startupDone = 1;
                        /* Clear the status bit*/
                        status = adi_metic_WriteRegister(pInfo, 0, ADE9178_REG_STATUS0, &status0);
                    }
                }
            }
            else
            {
                status = ADI_METIC_STATUS_IRQ0_NOT_RECEIVED;
            }
        }
    }

    return status;
}

ADI_METIC_STATUS CheckIntegrityError(ADI_METIC_INFO *pInfo)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    if ((pInfo->isHostRdy == 1) && (pInfo->isHostErr == 1))
    {
        status = ADI_METIC_STATUS_IC_INTEGRITY_ERROR;
    }
    return status;
}

ADI_METIC_STATUS CheckStartupError(ADI_METIC_INFO *pInfo)
{
    int32_t status1;

    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    if ((pInfo->isHostRdy == 0) && (pInfo->isHostErr == 1))
    {
        status = adi_metic_ReadRegister(pInfo, 0, ADE9178_REG_STATUS1, 1, &status1);
        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            if ((status1 & ADE9178_BITM_STATUS1_ERROR) != 0)
            {
                status1 = ADE9178_BITM_STATUS1_ERROR;
                status = adi_metic_ReadRegister(pInfo, 0, ADE9178_REG_ERROR_STATUS, 1, &status1);

                /*Do not clear the startup error so that users can cross check that if required*/
                if (status == ADI_METIC_STATUS_SUCCESS)
                {
                    pInfo->errorStatus = status1;
                    status = ADI_METIC_STATUS_STARTUP_ERROR;
                }
            }
        }
    }
    return status;
}

ADI_METIC_STATUS adi_metic_StartAdc(ADI_METIC_HANDLE hAde)
{
    int32_t config;
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        if (pInfo->startupDone == 0)
        {
            status = adi_metic_CheckStartupStatus(pInfo);
        }
        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            config = ADE9178_REG_ADC_CONFIG_RESET | ADE9178_BITM_ADC_CONTROL_ADC_INIT |
                     ADE9178_BITM_ADC_CONTROL_ADC_RUN;
            status = adi_metic_WriteRegister(pInfo, 0, ADE9178_REG_ADC_CONTROL, &config);
        }
    }
    return status;
}

ADI_METIC_STATUS adi_metic_HostRdyCallback(ADI_METIC_HANDLE hAde, uint8_t pinState)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        ADI_METIC_MASTER_EVENT_TYPE eventType = ADI_METIC_MASTER_EVENT_TYPE_HOST_RDY;
        if (pinState == 0)
        {
            eventType = ADI_METIC_MASTER_EVENT_TYPE_RESP_COMPLETED;
        }
        MeticCallBack(eventType, pInfo);
    }
    return status;
}

ADI_METIC_STATUS adi_metic_HostErrCallback(ADI_METIC_HANDLE hAde, uint8_t pinState)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        ADI_METIC_MASTER_EVENT_TYPE eventType = ADI_METIC_MASTER_EVENT_TYPE_HOST_ERR;

        if (pinState == 0)
        {
            eventType = ADI_METIC_MASTER_EVENT_TYPE_RESP_COMPLETED;
        }
        MeticCallBack(eventType, pInfo);
    }

    return status;
}

ADI_METIC_STATUS adi_metic_Irq0Callback(ADI_METIC_HANDLE hAde)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo->irq0Ready = 1;
    }

    return status;
}

void MeticCallBack(ADI_METIC_MASTER_EVENT_TYPE eventType, void *pData)
{
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)pData;
    pInfo->meticConfig.pfResume(pInfo->meticConfig.hUser, &pInfo->suspendState);
    if (eventType == ADI_METIC_MASTER_EVENT_TYPE_HOST_RDY)
    {
        pInfo->isHostRdy = 1;
    }
    else if (eventType == ADI_METIC_MASTER_EVENT_TYPE_HOST_ERR)
    {
        pInfo->isHostErr = 1;
    }
}

/**
 * @}
 */
