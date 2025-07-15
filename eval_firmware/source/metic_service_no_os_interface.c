/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_service_no_os_interface.c
 * @brief       Interface file for initialising IC.
 * @{
 */
/*============= I N C L U D E S =============*/

#include "app_cfg.h"
#include "metic_service_interface.h"
#include <stddef.h>
#include <stdint.h>

/*=============  C O D E  =============*/

int32_t MetIcIfSuspend(void *pInfo, volatile uint8_t *pSuspendState)
{
    int32_t status = 1;
    uint32_t waitCount = 0;
    if (pInfo != NULL)
    {
        status = 0;
        while ((*pSuspendState == 1) && (waitCount < APP_CFG_TIMEOUT_COUNT))
        {
            waitCount++;
        }
        *pSuspendState = 1;
        if (waitCount == APP_CFG_TIMEOUT_COUNT)
        {
            status = -1;
        }
    }

    return status;
}

int32_t MetIcIfResume(void *pInfo, volatile uint8_t *pSuspendState)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        *pSuspendState = 0;
    }

    return status;
}

/**
 * @}
 */
