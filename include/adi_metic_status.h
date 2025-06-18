/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file         adi_metic_status.h
 * @brief        Status Codes of Metrology Service
 * @addtogroup ADI_METIC
 * @{
 */

#ifndef __ADI_METIC_STATUS_H__
#define __ADI_METIC_STATUS_H__

/*=============  I N C L U D E S   =============*/

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N E S =============*/

/** @defgroup   METICSTATUS Return Codes

    @brief Metrology Service returns #ADI_METIC_STATUS for every API
 call. The application can look at the return codes and can take appropriate
 action for recovery. If multiple errors are there, the API will return  the
    return code of first encountered error.

    If an API returns an error, its description in the doxygen documentation
    gives hints at the cause of the error.

    Errors detected at development time
    regardless of the type of error, all errors should be identified and only
    ignored if the cause is known and understood, including informational
    errors.
    - Identify the API that returns the error.
    - Look at the doxygen description of the API that returned the error.
    - Ensure that any recommendations in the API description are followed.

    @note It is recommended that at the development stage all errors are
    evaluated, even the ones that can be ignored once the product is deployed,
    since they may help debugging issues in the application. Look into each
    error to determine the possible causes in each API.
 * @{
 */

/**
 * Status Codes of Metrology Service
 */

typedef enum
{
    /** Success */
    ADI_METIC_STATUS_SUCCESS,
    /** Handle is a null pointer*/
    ADI_METIC_STATUS_NULL_PTR,
    /** State memory provided to #adi_metic_Create is less than required memory
     * for the configuration.
     */
    ADI_METIC_STATUS_INSUFFICIENT_STATE_MEMORY,
    /** Error in creating instance of SPI. */
    ADI_METIC_STATUS_SCOMM_INIT_ERROR,
    /** Metrology IC returned an error */
    ADI_METIC_STATUS_METIC_RETURNED_ERROR,
    /** No response from Metrology IC */
    ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC,
    /** Number of registers passed is more than maximum registers #ADI_METIC_MAX_NUM_REGISTERS
       specified in Host*/
    ADI_METIC_STATUS_INVALID_NUM_REGISTERS,
    /** CRC mismatch in response received from Metrology IC */
    ADI_METIC_STATUS_FRAME_CRC_ERROR,
    /** Failed to send command to Metrology IC */
    ADI_METIC_STATUS_COMM_ERROR,
    /** previous transaction is still in progress.*/
    ADI_METIC_STATUS_WFS_PREV_TRANSACTION_IN_PROGRESS,
    /** RX DMA transaction is completed.*/
    ADI_METIC_STATUS_WFS_TRANSACTION_COMPLETED,
    /** Error in setting baudrate of UART*/
    ADI_METIC_STATUS_WFS_UART_BAUDRATE_ERROR,
    /** Error in receiving samples through UART*/
    ADI_METIC_STATUS_WFS_UART_COMM_ERROR,
    /** Metrology IC startup is not completed*/
    ADI_METIC_STATUS_STARTUP_PENDING,
    /** IRQ0 is not received from Metrology IC. It's recommended to reset the IC from application*/
    ADI_METIC_STATUS_IRQ0_NOT_RECEIVED,
    /** Metrology IC integrity error*/
    ADI_METIC_STATUS_IC_INTEGRITY_ERROR,
    /** Errors a startup time*/
    ADI_METIC_STATUS_STARTUP_ERROR,
    /** No valid samples in WFS rx buffer */
    ADI_METIC_STATUS_NO_VALID_SAMPLES,
    /** Configuration of WFS is disabled. Check the bitfields of enable and channelEn of WFS
       Register. */
    ADI_METIC_STATUS_WFS_DISABLED,
    /** Dummy code to decide end of the enums.
     *  Add all error codes above this*/
    ADI_METIC_STATUS_LAST_ERROR

} ADI_METIC_STATUS;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __ADI_METIC_STATUS_H__ */

/**
 * @}
 */
