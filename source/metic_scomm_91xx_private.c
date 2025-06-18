/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_scomm_91xx_private.c
 * @brief       Definitions for sending command to Metrology IC, and waiting till the response is
 * received.
 * @{
 */

/*==========================  I N C L U D E S   ==========================*/

#include "adi_metic.h"
#include "adi_metic_private.h"
#include "adi_metic_status.h"
#include "app_cfg.h"
#include <stddef.h>
#include <stdint.h>

static ADI_METIC_STATUS SendCmd(ADI_METIC_INFO *pInfo, uint8_t device, uint16_t addr, uint16_t cmd,
                                uint32_t numRegisters, int32_t data);
static ADI_METIC_STATUS GetResponse(ADI_METIC_INFO *pInfo, uint16_t cmd, uint32_t numRegisters,
                                    int32_t *pReceivedData, uint32_t *pNumReceivedBytes);

ADI_METIC_STATUS adi_metic_SendCmdGetResponse(ADI_METIC_HANDLE hAde, uint8_t device, uint16_t addr,
                                              uint16_t cmd, uint32_t numRegisters,
                                              int32_t *pWriteVal, int32_t *pReceivedData,
                                              uint32_t *pNumReceivedBytes)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    int32_t waitStatus = 0;
    ADI_METIC_INFO *pInfo;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo = (ADI_METIC_INFO *)hAde;
        pInfo->suspendState = 1;
        status = SendCmd(pInfo, device, addr, cmd, numRegisters, *pWriteVal);

        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            waitStatus =
                pInfo->meticConfig.pfSuspend(pInfo->meticConfig.hUser, &pInfo->suspendState);
            if (waitStatus != 0)
            {
                status = ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC;
            }
        }

        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            status = GetResponse(pInfo, cmd, numRegisters, pReceivedData, pNumReceivedBytes);
        }

        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            waitStatus =
                pInfo->meticConfig.pfSuspend(pInfo->meticConfig.hUser, &pInfo->suspendState);
            if (waitStatus != 0)
            {
                status = ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC;
            }
        }

        if (status == ADI_METIC_STATUS_SUCCESS)
        {
            status = pInfo->meticConfig.pfVerifyRespCrc(
                pInfo->meticConfig.hUser, (uint8_t *)pReceivedData, *pNumReceivedBytes);
            if (status != 0)
            {
                status = ADI_METIC_STATUS_FRAME_CRC_ERROR;
            }
            else
            {
                if (pInfo->isHostErr == 1)
                {
                    status = ADI_METIC_STATUS_METIC_RETURNED_ERROR;
                }
            }
        }
    }
    return status;
}

ADI_METIC_STATUS SendCmd(ADI_METIC_INFO *pInfo, uint8_t device, uint16_t addr, uint16_t cmd,
                         uint32_t numRegisters, int32_t data)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    int32_t transferStatus;
    uint32_t numCmdBytes = sizeof(ADI_METIC_CMD);
    pInfo->isHostRdy = 0;
    pInfo->isHostErr = 0;
    ADI_METIC_CMD *pCmd = &pInfo->cmd;
    pCmd->addr = addr;
    pCmd->rwb = cmd & 0x1;
    pCmd->device = device & 0x7;
    pCmd->numRegisters = numRegisters;
    pCmd->data = data;
    status = pInfo->meticConfig.pfAddCmdCrc(pInfo->meticConfig.hUser, (uint8_t *)pCmd,
                                            numCmdBytes - ADI_METIC_NUM_CRC_BYTES);

    if (pInfo->meticConfig.pfCmdTransfer != NULL)
    {
        transferStatus = pInfo->meticConfig.pfCmdTransfer(pInfo->meticConfig.hUser, (uint8_t *)pCmd,
                                                          numCmdBytes);
        if (transferStatus != 0)
        {
            status = ADI_METIC_STATUS_COMM_ERROR;
        }
    }
    else
    {
        status = ADI_METIC_STATUS_COMM_ERROR;
    }
    return status;
}

ADI_METIC_STATUS GetResponse(ADI_METIC_INFO *pInfo, uint16_t cmd, uint32_t numRegisters,
                             int32_t *pReceivedData, uint32_t *pNumReceivedBytes)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    int32_t receiveStatus = 0;
    int32_t numBytes = 0;
    if ((cmd == ADI_METIC_CMD_WRITE_REGISTER) || (pInfo->isHostErr == 1))
    {
        *pNumReceivedBytes = sizeof(uint32_t);
    }
    else
    {
        *pNumReceivedBytes = numRegisters * sizeof(uint32_t);
    }
    numBytes = *pNumReceivedBytes + ADI_METIC_NUM_CRC_BYTES;
    if (pInfo->meticConfig.pfResponseReceive != NULL)
    {
        receiveStatus = pInfo->meticConfig.pfResponseReceive(pInfo->meticConfig.hUser,
                                                             (uint8_t *)pReceivedData, numBytes);

        if (receiveStatus != 0)
        {
            status = ADI_METIC_STATUS_COMM_ERROR;
        }
    }
    else
    {
        status = ADI_METIC_STATUS_COMM_ERROR;
    }

    return status;
}

/**
 * @}
 */
