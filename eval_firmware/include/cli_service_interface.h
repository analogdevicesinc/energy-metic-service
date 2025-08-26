/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file      cli_service_interface.h
 * @brief     CLI interface
 * @{
 */

#ifndef __CLI_SERVICE_INTERFACE_H__
#define __CLI_SERVICE_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/

#include "adi_cli.h"
#include "adi_cli_memory.h"
#include "adi_cli_status.h"
#include <stdint.h>

/*=============  D E F I N I T I O N S  =============*/

/*============= D A T A  T Y P E S =============*/

/**
 * Structure to hold data for user handle.
 */
typedef struct
{
    /** CLI configurations */
    ADI_CLI_CONFIG config;
    /** handle for CLI */
    ADI_CLI_HANDLE hCli;
    /** Library memory for CLI Service */
    uint32_t stateMemory[ADI_CLI_STATE_MEM_NUM_BYTES / 4];
    /** temporary memory for CLI Service */
    uint32_t tempMemory[ADI_CLI_TEMP_MEM_NUM_BYTES / 4];
    /** Command buffer */
    char command[APP_CFG_CLI_MAX_CMD_LENGTH];
} CLI_INTERFACE_INFO;

/*======= P U B L I C   P R O T O T Y P E S ========*/

/**
 * @brief Creates CLI instance
 * @param[in]  pInfo  - User instance Handle
 * @return  success or error
 */
ADI_CLI_STATUS CliCreateInstance(CLI_INTERFACE_INFO *pInfo);

#ifdef __cplusplus
}
#endif

#endif /* __CLI_SERVICE_INTERFACE_H__  */

/**
 * @}
 */
