/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_hal.h
 * @brief       Hardware abstraction layer for the functionality of Metrology Service
 * @addtogroup  ADI_SCOMM
 * @{
 */

#ifndef __METIC_HAL__
#define __METIC_HAL__

/*============= I N C L U D E S =============*/

#ifdef __cplusplus
extern "C" {
#endif

/*============= I N C L U D E S =============*/

#include "adi_metic.h"
#include <stdint.h>

/*======= P U B L I C   P R O T O T Y P E S ========*/

/**
 * @brief Function to assign user instance to local handle.
 * @param[in] hInstance 		- User instance Handle
 *
 */
void AdeInitHal(ADI_METIC_HANDLE hInstance);

/**
 * @brief Function to handle HOST_RDY callback
 * @param[in] flag 		- status of GPIO
 *
 */
void AdeHandleHostRdyErrCallback(uint32_t flag);

#ifdef __cplusplus
}
#endif

#endif /* __ADE_HAL__ */

/**
 * @}
 */
