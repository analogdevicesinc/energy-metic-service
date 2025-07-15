/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_service_adapter.c
 * @brief       Adapter file.
 * @{
 */

/*============= I N C L U D E S =============*/
#include "ade_crc.h"
#include "adi_ade9178_cmd_format.h"
#include "adi_evb.h"
#include "adi_metic.h"
#include "metic_service_interface.h"
#include <stdint.h>
#include <string.h>

/*============= F U N C T I O N S =============*/

/**
 * @brief Function to start SPI transmission. #ADI_METIC_CONFIG.pfTransfer to be initialised with
 * this function.
 * @param[in] pInfo 		- User Handle
 * @param[in] pData   -  pointer to data
 * @param[in] numBytes      -  number of bytes to transmit
 * @returns error if user info is NULL, else success
 */
static int32_t AdeSpiTransmitAsync(void *pInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Function to start SPI reception. #ADI_METIC_CONFIG.pfReceive to be initialised with this
 * function.
 * @param[in] pInfo 		- User Handle
 * @param[in] pData   -  pointer to data
 * @param[in] numBytes      -  number of bytes to receive
 * @returns error if user info is NULL, else success
 */
static int32_t AdeSpiReceiveAsync(void *pInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Function to set uart baudrate. #ADI_METIC_CONFIG.pfSetBaudRate to be initialised with this
 * function.
 * @param[in] pInfo 		- User Handle
 * @param[in] baudRate   -  baudrate of UART
 * @returns error if user info is NULL, else success
 */
static int32_t AdeUartSetBaudrate(void *pInfo, uint32_t baudRate);

/**
 * @brief Function to start uart reception. #ADI_METIC_CONFIG.pfStartUartRx to be initialised with
 * this function.
 * @param[in] pInfo 		- User Handle
 * @param[in] pData   -  pointer to data
 * @param[in] numBytes   -  number of bytes to receive
 * @returns error if user info is NULL, else success
 */
static int32_t AdetUartReceiveAsync(void *pInfo, uint8_t *pData, uint32_t numBytes);
static uint16_t AddCmdCrc(void *pInfo, uint8_t *pData, uint32_t numBytes);
static uint16_t VerifyRespCrc(void *pInfo, uint8_t *pData, uint32_t numBytes);

void MetIcAdaptPopulateConfig(ADI_METIC_CONFIG *pConfig)
{

    pConfig->pfCmdTransfer = AdeSpiTransmitAsync;
    pConfig->pfResponseReceive = AdeSpiReceiveAsync;
    pConfig->pfWfrmReceive = AdetUartReceiveAsync;
    pConfig->pfResume = MetIcIfResume;
    pConfig->pfSuspend = MetIcIfSuspend;
    pConfig->pfSetBaudRate = AdeUartSetBaudrate;
    pConfig->pfAddCmdCrc = AddCmdCrc;
    pConfig->pfVerifyRespCrc = VerifyRespCrc;
    pConfig->pfClose = NULL;
}

uint16_t AddCmdCrc(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    uint16_t crc = 0;
    if (pInfo != NULL)
    {
        crc = AdeCalculateCrc16(pData, numBytes);
    }
    return crc;
}

uint16_t VerifyRespCrc(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    uint16_t status = 0;
    uint32_t expectedCrc = 0;
    if (pInfo != NULL)
    {
        expectedCrc = pData[numBytes];
        expectedCrc |= pData[numBytes + 1] << 8;
        // Calculate the CRC of the received frame (excluding CRC field)
        uint16_t responseCrc = AdeCalculateCrc16(pData, numBytes);

        if (expectedCrc != responseCrc)
        {
            status = 1;
        }
    }
    return status;
}

int32_t AdeSpiTransmitAsync(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbAdeSpiTransmitAsync(pInfo, pData, numBytes);
    }

    return status;
}

int32_t AdeSpiReceiveAsync(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbAdeSpiReceiveAsync(pInfo, pData, numBytes);
    }

    return status;
}

int32_t AdeUartSetBaudrate(void *pInfo, uint32_t baudRate)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbAdeWfsUartSetBaudrate(pInfo, baudRate);
    }

    return status;
}

int32_t AdetUartReceiveAsync(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbAdeWfsUartReceiveAsync(pInfo, pData, numBytes);
    }
    return status;
}

/**
 * @}
 */
