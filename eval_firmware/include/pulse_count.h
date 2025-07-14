/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file pulse_count.h
 * @brief    Definitions needed for pulse counter intialisation,storing and getting pulses.
 * @addtogroup ADI_ADE
 * @{
 */

#ifndef __PULSE_COUNT_H__
#define __PULSE_COUNT_H__

/*=============  I N C L U D E S   =============*/

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N E S =============*/

/**
 * Structure to hold pulses counters
 */
typedef struct
{
    /** Number of pulses that can be stored in the buffer */
    uint32_t len;
    /** Location in buffer where the next pulse data is to be stored */
    uint32_t writeIndex;
    /** Location in buffer where the next pulse data is to be read */
    uint32_t readIndex;
    /** Pointer to buffer where pulse data is stored */
    uint32_t *pBuffer;
} PCNT_INFO;

/**
 * @brief  Intialises pulse counter data
 * @param[in] pInfo	  - Pointer to structure storing pulse counter data
 * @param[in] numPulses	  - number of pulses to store in the output buffer
 * @param[out] pOutput	  - Pointer to output buffer
 *
 */
void PcntInit(PCNT_INFO *pInfo, uint32_t numPulses, uint32_t *pOutput);

/**
 * @brief  Copies pulse counter internal data to output buffer.
 * @param[in] pInfo	  - Pointer to structure storing pulse counter data
 * @param[out] pOutput	  - Pointer to output buffer
 *
 */
uint32_t PcntCopyBuffer(PCNT_INFO *pInfo, uint32_t *pOutput);

/**
 * @brief  Adds stat, pulse time data to pulse counter buffer
 * @param[in] pInfo	  - Pointer to structure storing pulse counter data
 * @param[in] stat	  - status of GPIO pin
 * @param[in] time	  - time at which pulse occured.
 *
 */
void PcntAddToBuffer(PCNT_INFO *pInfo, uint32_t stat, uint32_t time);

#ifdef __cplusplus
}
#endif

#endif /* __PULSE_COUNT_H__ */

/**
 * @}
 */
