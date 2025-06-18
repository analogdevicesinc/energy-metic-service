/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_service_init_interface.c
 * @brief       Interface file for initialising IC.
 * @{
 */
/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "adi_metic.h"
#include "app_cfg.h"
#include "metic_hal.h"
#include "metic_service_interface.h"
#include <stddef.h>
#include <stdint.h>
/** User handle pointer*/
static METIC_INSTANCE_INFO *pAdeInstance;

/**
 * @brief Configures ADE9178 with recommended values by user
 * @param[in] pInfo	  - pointer to user handle
 */
static ADI_METIC_STATUS ConfigureAde9178(METIC_INSTANCE_INFO *pInfo);

/*=============  C O D E  =============*/

ADI_METIC_STATUS MetIcIfCreateInstance(METIC_INSTANCE_INFO *pInfo)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_CONFIG *pConfig = &pInfo->config;
    void *pStateMemory = &pInfo->stateMemory[0];
    uint32_t stateMemSize = sizeof(pInfo->stateMemory);

    pInfo->integrityStatus = 0;
    status = adi_metic_Create(&pInfo->hAde, pStateMemory, stateMemSize);
    if (status == ADI_METIC_STATUS_SUCCESS)
    {
        MetIcAdaptPopulateConfig(pConfig);
        pConfig->hUser = pInfo;
        status = adi_metic_Init(pInfo->hAde, pConfig);
    }
    /* At this point Met IC initialization is complete. Following codes setup some
     * pointers in example before enabling data capture*/
    if (status == ADI_METIC_STATUS_SUCCESS)
    {
        AdeInitHal(pInfo->hAde);
        pAdeInstance = pInfo;
        pAdeInstance->cfIndex = 0;
        EvbStartTimer();
        EvbEnableAllGPIOIrq();
    }
    return status;
}

ADI_METIC_STATUS MetIcIfStartAdc(METIC_INSTANCE_INFO *pInfo)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    uint32_t currTime = 0;
    uint32_t startTime = 0;
    /** Wait till startup is done */
    startTime = EvbGetTime();
    currTime = EvbGetTime();

    while ((currTime - startTime) < APP_CFG_STARTUP_TIMEOUT_COUNT)
    {
        currTime = EvbGetTime();
        status = adi_metic_CheckStartupStatus(pInfo->hAde);
        if ((status == ADI_METIC_STATUS_SUCCESS) || (status == ADI_METIC_STATUS_IC_INTEGRITY_ERROR))
        {
            break;
        }
    }
    if (status == ADI_METIC_STATUS_SUCCESS)
    {
        status = ConfigureAde9178(pInfo);
    }

    if (status == ADI_METIC_STATUS_SUCCESS)
    {
        status = adi_metic_StartAdc(pInfo->hAde);
    }
    return status;
}

ADI_METIC_STATUS ConfigureAde9178(METIC_INSTANCE_INFO *pInfo)
{
    int32_t i;
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    static uint16_t registers[6] = {ADE9178_REG_CONFIG0,    ADE9178_REG_ZXTHRSH,
                                    ADE9178_REG_CF1_CONFIG, ADE9178_REG_CF1_THR,
                                    ADE9178_REG_EP_CFG,     ADE9178_REG_EGY_TIME};
    static int32_t values[6] = {APP_CFG_ADE9178_CONFIG0,    APP_CFG_ADE9178_ZXTHRSH,
                                APP_CFG_ADE9178_CF1_CONFIG, APP_CFG_ADE9178_CF1_THR,
                                APP_CFG_ADE9178_EP_CFG,     APP_CFG_ADE9178_EGY_TIME};
    int32_t numRegisters = 6;

    for (i = 0; i < numRegisters; i++)
    {
        status = adi_metic_WriteRegister(pInfo->hAde, 0, registers[i], &values[i]);
        if (status != ADI_METIC_STATUS_SUCCESS)
        {
            break;
        }
    }
    return status;
}

void MetIcIfUpdateIrqStatus(uint32_t flag)
{
    ADE_IRQ_STATUS *pIrqStatus = &pAdeInstance->irqStatus;
    uint32_t time;
    if ((flag & BOARD_CFG_IRQ0_PIN) != 0)
    {
        pIrqStatus->irq0Count++;
        pIrqStatus->irq0Ready = 1;
        adi_metic_Irq0Callback(pAdeInstance->hAde);
    }

    if ((flag & BOARD_CFG_IRQ1_PIN) != 0)
    {
        pIrqStatus->irq1Ready = 1;
    }
    if ((flag & BOARD_CFG_IRQ2_PIN) != 0)
    {
        pIrqStatus->irq2Ready = 1;
    }
    if ((flag & BOARD_CFG_IRQ3_PIN) != 0)
    {
        pIrqStatus->irq3Ready = 1;
    }
    if (((flag & BOARD_CFG_CF1_PIN) != 0) || ((flag & BOARD_CFG_CF2_PIN) != 0))
    {
        time = EvbGetTime();
        pAdeInstance->cfTime[pAdeInstance->cfIndex] = time;
        pAdeInstance->cfFlag[pAdeInstance->cfIndex] = flag;
        if (pAdeInstance->cfIndex >= APP_CFG_MAX_NUM_IRQ_TIME)
        {
            pAdeInstance->cfIndex = 0;
        }
        pAdeInstance->cfIndex++;
    }
}

void MetIcIfGPIOCallback(uint32_t port, uint32_t flag)
{
    if (port == (uint32_t)BOARD_CFG_ADECOMM_PORT)
    {
        AdeHandleHostRdyErrCallback(flag);
    }
    if (port == (uint32_t)BOARD_CFG_ADEIRQ_PORT)
    {
        MetIcIfUpdateIrqStatus(flag);
    }
}

void MetIcIfWfsUartCallBack(void)
{
    adi_metic_WfsUartRxCallback(pAdeInstance->hAde);
    pAdeInstance->isWfsRxComplete = 1;
}

void MetIcIfClose(METIC_INSTANCE_INFO *pInfo)
{
    adi_metic_Close(pInfo->hAde);
}

/**
 * @}
 */
