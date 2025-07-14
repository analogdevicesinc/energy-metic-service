/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_service_adapter.c
 * @brief       Adapter file.
 * @{
 */

/*============= I N C L U D E S =============*/
#include "adi_crc.h"
#include "adi_crc_ccitt16.h"
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
static int32_t AdeStartSpiTx(void *pInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Function to start SPI reception. #ADI_METIC_CONFIG.pfReceive to be initialised with this
 * function.
 * @param[in] pInfo 		- User Handle
 * @param[in] pData   -  pointer to data
 * @param[in] numBytes      -  number of bytes to receive
 * @returns error if user info is NULL, else success
 */
static int32_t AdeStartSpiRx(void *pInfo, uint8_t *pData, uint32_t numBytes);

/**
 * @brief Function to set uart baudrate. #ADI_METIC_CONFIG.pfSetBaudRate to be initialised with this
 * function.
 * @param[in] pInfo 		- User Handle
 * @param[in] baudRate   -  baudrate of UART
 * @returns error if user info is NULL, else success
 */
static int32_t AdeSetUartBaudrate(void *pInfo, uint32_t baudRate);

/**
 * @brief Function to start uart reception. #ADI_METIC_CONFIG.pfStartUartRx to be initialised with
 * this function.
 * @param[in] pInfo 		- User Handle
 * @param[in] pData   -  pointer to data
 * @param[in] numBytes   -  number of bytes to receive
 * @returns error if user info is NULL, else success
 */
static int32_t AdeStartUartRx(void *pInfo, uint8_t *pData, uint32_t numBytes);
static int32_t AddCmdCrc(void *pInfo, uint8_t *pData, uint32_t numBytes);
static int32_t VerifyRespCrc(void *pInfo, uint8_t *pData, uint32_t numBytes);

void MetIcAdaptPopulateConfig(ADI_METIC_CONFIG *pConfig)
{

    pConfig->pfCmdTransfer = AdeStartSpiTx;
    pConfig->pfResponseReceive = AdeStartSpiRx;
    pConfig->pfWfrmReceive = AdeStartUartRx;
    pConfig->pfResume = MetIcIfResume;
    pConfig->pfSuspend = MetIcIfSuspend;
    pConfig->pfSetBaudRate = AdeSetUartBaudrate;
    pConfig->pfAddCmdCrc = AddCmdCrc;
    pConfig->pfVerifyRespCrc = VerifyRespCrc;
    pConfig->pfClose = NULL;
}

int32_t AddCmdCrc(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 0;
    if (pInfo != NULL)
    {
        adi_crc_AddCCITT16(pData, numBytes);
    }
    else
    {
        status = 1;
    }
    return status;
}

int32_t VerifyRespCrc(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    ADI_CRC_RESULT crcStatus;
    int32_t status = 0;
    if (pInfo != NULL)
    {
        crcStatus = adi_crc_VerifyCCITT16(pData, numBytes);
        if (crcStatus != ADI_CRC_RESULT_SUCCESS)
        {
            status = 1;
        }
    }
    else
    {
        status = 1;
    }
    return status;
}

int32_t AdeStartSpiTx(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbStartAdeSpiTx(pInfo, pData, numBytes);
    }

    return status;
}

int32_t AdeStartSpiRx(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbStartAdeSpiRx(pInfo, pData, numBytes);
    }

    return status;
}

int32_t AdeSetUartBaudrate(void *pInfo, uint32_t baudRate)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbSetAdeWfsUartBaudrate(pInfo, baudRate);
    }

    return status;
}

int32_t AdeStartUartRx(void *pInfo, uint8_t *pData, uint32_t numBytes)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        status = EvbStartAdeWfsUartRx(pInfo, pData, numBytes);
    }
    return status;
}

/**
 * @}
 */
