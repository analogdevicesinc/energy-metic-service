/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file adi_metic_private.h
 * @brief    Structures to hold Metrology Service Data and Configurations
 * @{
 */

#ifndef __ADI_METIC_PRIVATE_H__
#define __ADI_METIC_PRIVATE_H__

/*=============  I N C L U D E S   =============*/

#include "adi_metic.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Waveform Stream Configuration register
 */
typedef struct
{
    /** Configuration of WFS register */
    ADI_METIC_WFS_CONFIG config;
    /** WFS RX completion flag */
    uint32_t isWfsRxComplete;
    /** buffer to store channels enabled in WFS */
    uint8_t channelIdEnabledBuff[ADI_METIC_MAX_NUM_CHANNELS];
} ADI_METIC_WFS_INFO;

/**
 * Structure to hold data required for metrology service
 */
typedef struct
{
    /** stores data required for WFS */
    ADI_METIC_WFS_INFO wfsData;
    /** configurations of metrology service */
    ADI_METIC_CONFIG meticConfig;
    /** suspend state for read/write operation*/
    uint8_t suspendState;
    /** buffer to store read register outputs */
    int32_t responseBuffer[ADI_METIC_MAX_NUM_REGISTERS + 1];
    /** host error flag */
    uint8_t isHostErr;
    /** host ready flag */
    uint8_t isHostRdy;
    /** irq0 ready */
    uint8_t irq0Ready;
    /**  startup is complete */
    uint8_t startupDone;
    /** commad format */
    ADI_METIC_CMD cmd;
    /** Last error status */
    int32_t errorStatus;

} ADI_METIC_INFO;

/**
 * @brief Sends command and gets response from Metrology IC and verifies CRC.
 * @param[in] hMetIc 		- Metrology service handle
 * @param[in] device    -  Device Id.
 *                         0 - ADE9178,
 *                         1 - ADC1,
 *                         2 - ADC2,
 *                         3 - ADC3,
 *                         4 - ADC4,
 *                         5 - ALL_ADC,
 * @param[in] addr   - address of the register
 * @param[in] cmd        - command to indicate  whether its write or read
 *                       0 - WRITE,
 *                       1 - READ,
 * @param[in] numRegisters   - number of registers to read or write
 * @param[in] pData   - pointer to data to be written
 * @param[out] pReceivedData   - data response received from Metrology IC
 * @param[out] pNumReceivedBytes   - number of bytes received from Metrology IC.
 * @returns #ADI_METIC_STATUS_SUCCESS on success or #ADI_METIC_STATUS_NULL_PTR,
 * #ADI_METIC_STATUS_FRAME_CRC_ERROR, #ADI_METIC_STATUS_METIC_RETURNED_ERROR,
 * #ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC on failure.
 */
ADI_METIC_STATUS adi_metic_SendCmdGetResponse(ADI_METIC_HANDLE hMetIc, uint8_t device,
                                              uint16_t addr, uint16_t cmd, uint32_t numRegisters,
                                              int32_t *pData, int32_t *pReceivedData,
                                              uint32_t *pNumReceivedBytes);

#ifdef __cplusplus
}
#endif

#endif /* __ADI_METIC_PRIVATE_H__ */

/** @} */
