/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     cli_service_interface.c
 * @{
 */

/*=============  I N C L U D E S   =============*/
#include "cli_service_interface.h"
#include "adi_cli.h"
#include "adi_evb.h"
#include "dispatch_table.h"
#include <stdlib.h>

/*=============  D A T A  =============*/

static int32_t CliStartRxAsync(void *pInfo, char *pData, uint32_t numBytes);
static int32_t CliStartTxAsync(void *pInfo, uint8_t *pData, uint32_t numBytes);
static void PopulateCliConfig(CLI_INTERFACE_INFO *pInfo);

/*============= C O D E =============*/

ADI_CLI_STATUS CliCreateInstance(CLI_INTERFACE_INFO *pInfo)
{
    ADI_CLI_STATUS status;
    ADI_CLI_CONFIG *pConfig = &pInfo->config;
    PopulateCliConfig(pInfo);

    status = adi_cli_Create(&pInfo->hCli, pInfo->stateMemory, sizeof(pInfo->stateMemory),
                            pInfo->tempMemory, sizeof(pInfo->tempMemory));

    if (status == 0)
    {
        status = adi_cli_Init(pConfig);
    }
    return status;
}

void PopulateCliConfig(CLI_INTERFACE_INFO *pInfo)
{
    ADI_CLI_CONFIG *pConfig = &pInfo->config;
    pConfig->pfTransmitAsync = CliStartTxAsync;
    pConfig->pfReceiveAsync = CliStartRxAsync;
    pConfig->pDispatchTable = dispatchTable;
    pConfig->numRecords = NUM_COMMANDS;
}

int32_t CliStartRxAsync(void *pInfo, char *pData, uint32_t numBytes)
{
    int32_t status = 0;
    if (pInfo != NULL)
    {
        status = EvbStartHostUartRxAsync(pInfo, (uint8_t *)pData, numBytes);
    }
    return status;
}

int32_t CliStartTxAsync(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;
    if (pInfo != NULL)
    {
        status = EvbStartHostUartTxAsync(pInfo, pData, numBytes);
    }
    return status;
}

/**
 * @}
 */