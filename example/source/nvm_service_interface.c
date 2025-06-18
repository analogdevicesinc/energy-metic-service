/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/*==========================  I N C L U D E S   ==========================*/

#include "nvm_service_interface.h"
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

/** ADC interface info variable */
static NVM_INTERFACE_INFO *pNvmIf;

ADI_NVM_STATUS NvmCreateInstance(NVM_INTERFACE_INFO *pInfo)
{
    ADI_NVM_STATUS status;
    // This is the dummy configuration.
    ADI_NVM_CONFIG *pConfig = &(pInfo->config);
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
    pConfig->hUser = pInfo;
}

/**
 * @}
 */
