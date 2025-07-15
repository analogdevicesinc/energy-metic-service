/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file      nvm_service_interface.h
 * @brief     NVM interface
 * @{
 */

#ifndef __NVM_SERVICE_INTERFACE_H__
#define __NVM_SERVICE_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/

#include "adi_nvm.h"
#include "adi_nvm_memory.h"
#include "adi_nvm_status.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/

/*============= D A T A  T Y P E S =============*/

/**
 * Structure to hold data for user handle.
 */
typedef struct
{

    /** Library memory for NVM Service */
    uint32_t stateMemory[ADI_NVM_STATE_MEM_NUM_BYTES / 4];
    /** NVM configurations */
    ADI_NVM_CONFIG config;
    /** NVM handle */
    ADI_NVM_HANDLE hNvm;
    /** flag to indicate tx completion */
    volatile uint8_t isTxComplete;

} NVM_INTERFACE_INFO;

/*======= P U B L I C   P R O T O T Y P E S ========*/

/**
 * @brief Creates NVM instance
 * @param[in]  pInfo  - User instance Handle
 * @return  success or error
 */
ADI_NVM_STATUS NvmCreateInstance(NVM_INTERFACE_INFO *pInfo);

#ifdef __cplusplus
}
#endif

#endif /* __NVM_SERVICE_INTERFACE_H__  */

/**
 * @}
 */
