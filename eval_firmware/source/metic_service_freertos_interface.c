/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_service_freertos_interface.c
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
    int32_t status = 0;
    return status;
}

int32_t MetIcIfResume(void *pInfo, volatile uint8_t *pSuspendState)
{
    int32_t status = 0;
    return status;
}

/**
 * @}
 */
