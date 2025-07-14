/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/*==========================  I N C L U D E S   ==========================*/

#include "nvm_service_interface.h"
#include "ade_crc.h"
#include "adi_evb.h"
#include "adi_nvm.h"
#include "app_cfg.h"
#include "status.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @file        nvm_service_interface.c
 * @brief       Definitions for reading and writing to NVM.
 * @{
 */

/** NVM interface info variable */
static NVM_INTERFACE_INFO *pNvmIf;

static void PopulateNvmConfig(NVM_INTERFACE_INFO *pInfo);
static int32_t NvmFlashWrite(void *pInfo, uint8_t *pSrc, uint32_t numBytes);
static int32_t NvmFlashRead(void *pInfo, uint8_t *pSrc, uint32_t numBytes, uint8_t *pDst);
static uint16_t NvmCalculateCrc(void *pInfo, uint8_t *pData, uint32_t numBytes);

ADI_NVM_STATUS NvmCreateInstance(NVM_INTERFACE_INFO *pInfo)
{
    ADI_NVM_STATUS status;
    ADI_NVM_CONFIG *pConfig = &(pInfo->config);
    PopulateNvmConfig(pInfo);
    status = adi_nvm_Create(&pInfo->hNvm, pInfo->stateMemory, sizeof(pInfo->stateMemory));
    if (status == 0)
    {
        status = adi_nvm_Init(pInfo->hNvm, pConfig);
        pNvmIf = pInfo;
    }

    return status;
}

void PopulateNvmConfig(NVM_INTERFACE_INFO *pInfo)
{
    ADI_NVM_CONFIG *pConfig = &pInfo->config;

    pConfig->pfWrite = NvmFlashWrite;
    pConfig->pfRead = NvmFlashRead;
    pConfig->pfCalculateCrc = NvmCalculateCrc;
    pConfig->hUser = pInfo;
}

uint16_t NvmCalculateCrc(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    uint16_t crc = 0;
    if (pInfo != NULL)
    {
        crc = AdeCalculateCrc16(pData, numBytes);
    }
    return crc;
}

int32_t NvmFlashWrite(void *pInfo, uint8_t *pSrc, uint32_t numBytes)
{
    int32_t status = 0;
    if (pInfo != NULL)
    {
        status = EvbNvmWrite(pInfo, pSrc, numBytes, APP_CFG_TIMEOUT_COUNT);
    }
    return status;
}

int32_t NvmFlashRead(void *pInfo, uint8_t *pSrc, uint32_t numBytes, uint8_t *pDst)
{
    int32_t status = 0;
    if (pInfo != NULL)
    {
        status = EvbNvmRead(pInfo, pSrc, pDst, numBytes, APP_CFG_TIMEOUT_COUNT);
    }
    return status;
}

/**
 * @}
 */
