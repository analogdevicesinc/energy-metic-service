/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @addtogroup EXAMPLE
 * @{
 */

/**
 * @file status.h
 * @brief       Example system status codes
 * @defgroup  STATUS_CODES Status codes
 * @{
 */
#ifndef __STATUS_H__
#define __STATUS_H__

/*=============  I N C L U D E S   =============*/
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N E S =============*/

/**
 * @brief Example Status Enums
 *
 */
typedef enum
{
    /** Success */
    SYS_STATUS_SUCCESS = 0,
    /** Example for paramter capture not started */
    SYS_STATUS_NOT_STARTED = 1,
    /** Example is running  */
    SYS_STATUS_RUNNING = 2,
    /** Error from SPI module */
    SYS_STATUS_SPI_COMM_ERROR = 3,
    /** Error from UART module */
    SYS_STATUS_CLI_INIT_FAILED = 4,
    /** Time out for irq0 */
    SYS_STATUS_IRQ0_TIME_OUT_ERROR = 5,
    /** EVB init failure */
    SYS_STATUS_EVB_INIT_FAILURE = 6,
    /** ADE init failure */
    SYS_STATUS_ADE_INIT_FAILURE = 7,
    SYS_STATUS_ADE_UART_COMM_ERROR = 8,
    /** EXAMPLE init failure */
    SYS_STATUS_EXAMPLE_INIT_FAILURE = 9,
    /** Buffer is full. */
    SYS_STATUS_EXAMPLE_BUFFER_FULL = 10,
    SYS_STATUS_MET_PROCESS = 11,
} SYS_STATUS;

/**
 * @brief  Handles the error from various modules
 * @param[in] status - status code
 * @return status 0 for success
 */
int32_t HandleError(int32_t status);
/**
 * @brief  Read error status if error interrupt occurred.
 */
void ReadErrorStatus(void);

#ifdef __cplusplus
}
#endif

#endif /* __STATUS_H__ */

/**
 * @}
 */

/**
 * @}
 */
