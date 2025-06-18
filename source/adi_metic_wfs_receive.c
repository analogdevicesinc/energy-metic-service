/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     adi_metic_wfs_receive.c
 * @brief    This file contains the routines for capturing waveform samples from
 * Metrology IC
 * @{
 */

/*=============  I N C L U D E S   =============*/

#include "adi_metic.h"
#include "adi_metic_private.h"
#include "adi_metic_status.h"
#include <stdint.h>
#include <string.h>

/** Mask value of WFS_CONFIG register with only all channels enabled.*/
#define ADE9178_BITM_WFS_CONFIG_CHANNEL_EN 0x7ff80

/**
 * Gets baud rate based on configuration
 * @param[in]  baudRateBit - baudrate bits
 */
static uint32_t GetBaudRate(uint8_t baudRateBit);

/**
 * Returns next channel id enabled.
 * @param[in]  channelEnable - configuration of channel enable.
 * @param[in]  channelId - channel id.
 */

static int32_t GeNextChannelId(uint8_t *pChannelIdEnabledBuf, int32_t channelId,
                               int32_t numChannelsEnabled);

/**
 * Stores the channels enabled in WFS in a buffer.
 * @param[in]  channelSelect - configuration of channel select.
 * @param[in]  pDst - pointer to destination buffer.
 */
static void StoreChannelIdEnabled(int32_t channelSelect, uint8_t *pDst);

/**
 * Returns number of channels enabled.
 * @param[in]  channelEnable - configuration of channel enable.
 */
static uint16_t GetNumEnabledChannels(uint32_t channelEnable);

/**
 * Checks channels positions (every (N+4) pos) iterating through all the enabled channels.
 * @param[in]  pBuffer - pointer to input buffer.
 * @param[in]  pChannelIdEnabledBuff - pointer to buffer having channel id's.
 * @param[in]  startChannel - start channel id.
 * @param[in]  numChannelsEnabled - number of channels enabled.
 * @return 0 if all the channel ids positions are proper
 */
static int32_t CheckChannelPosition(int8_t *pBuffer, uint8_t *pChannelIdEnabledBuff,
                                    int32_t startChannel, int32_t numChannelsEnabled);

/*=============  C O D E  =============*/

ADI_METIC_STATUS adi_metic_WfsConfigureRx(ADI_METIC_HANDLE hAde, int32_t config)
{
    uint32_t baudRate;
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    int32_t wfsUartStatus;
    ADI_METIC_INFO *pInfo;
    ADI_METIC_WFS_ADE9178_REG_CONFIG *pWfsRegConfig;
    ADI_METIC_WFS_INFO *pWfsInfo;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo = (ADI_METIC_INFO *)hAde;
        pWfsInfo = &pInfo->wfsData;
        pWfsRegConfig = &pInfo->wfsData.config.wfsRegConfig;
        // copies wfs config data to appropriate bit fields.
        memcpy(pWfsRegConfig, &config, sizeof(ADI_METIC_WFS_ADE9178_REG_CONFIG));
        // Check if any of the channel is enabled.
        if ((pWfsRegConfig->enable == 1) && ((config & ADE9178_BITM_WFS_CONFIG_CHANNEL_EN) != 0))
        {
            StoreChannelIdEnabled(pWfsRegConfig->channelSelect, &pWfsInfo->channelIdEnabledBuff[0]);
            baudRate = GetBaudRate(pWfsRegConfig->baudrate);
            wfsUartStatus = pInfo->meticConfig.pfSetBaudRate(pInfo->meticConfig.hUser, baudRate);
            if (wfsUartStatus != 0)
            {

                status = ADI_METIC_STATUS_WFS_UART_BAUDRATE_ERROR;
            }
        }
    }
    return status;
}

ADI_METIC_STATUS adi_metic_WfsStartRx(ADI_METIC_HANDLE hAde, uint32_t numBytes, uint8_t *pSamples)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_WFS_TRANSACTION_COMPLETED;
    int32_t wfsUartStatus = 0;
    ADI_METIC_INFO *pInfo;
    int32_t config;
    ADI_METIC_WFS_ADE9178_REG_CONFIG wfsRegConfig;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo = (ADI_METIC_INFO *)hAde;
        wfsRegConfig = pInfo->wfsData.config.wfsRegConfig;
        memcpy(&config, &wfsRegConfig, sizeof(ADI_METIC_WFS_ADE9178_REG_CONFIG));
        if ((pInfo->wfsData.isWfsRxComplete == 1) && (wfsRegConfig.enable == 1))
        {
            pInfo->wfsData.isWfsRxComplete = 0;
            wfsUartStatus = pInfo->meticConfig.pfWfrmReceive(pInfo->meticConfig.hUser,
                                                             (uint8_t *)pSamples, numBytes);
            if (wfsUartStatus != 0)
            {
                status = ADI_METIC_STATUS_WFS_UART_COMM_ERROR;
            }
        }
        else if (pInfo->wfsData.isWfsRxComplete == 0)
        {
            status = ADI_METIC_STATUS_WFS_PREV_TRANSACTION_IN_PROGRESS;
        }
        else if ((wfsRegConfig.enable == 0) || ((config & ADE9178_BITM_WFS_CONFIG_CHANNEL_EN) != 0))
        {
            status = ADI_METIC_STATUS_WFS_DISABLED;
        }
    }
    return status;
}

ADI_METIC_STATUS adi_metic_FindChannelOffset(ADI_METIC_HANDLE hAde, int8_t *pBuffer,
                                             int32_t numBytes, int32_t channelId,
                                             int32_t *pByteOffset)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_NO_VALID_SAMPLES;
    ADI_METIC_WFS_CONFIG *pWfsConfig;
    ADI_METIC_WFS_INFO *pInfo;
    uint8_t *pChannelIdEnabledBuff;
    int32_t syncStatus;

    int32_t numGoodSamples = 0;
    int32_t requiredGoodSamples;
    int32_t numChannelsEnabled;
    uint32_t channelSelect;
    int32_t numBytesSkip = 0;
    int32_t currentPos = 0;
    int32_t numRemainingBytes = numBytes;

    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo = &((ADI_METIC_INFO *)hAde)->wfsData;
        pWfsConfig = &pInfo->config;
        pChannelIdEnabledBuff = &pInfo->channelIdEnabledBuff[0];
        channelSelect = pWfsConfig->wfsRegConfig.channelSelect;
        requiredGoodSamples = pWfsConfig->offsetCount;
        numChannelsEnabled = GetNumEnabledChannels(channelSelect);

        while (numRemainingBytes >= (numChannelsEnabled * 4))
        {
            syncStatus = CheckChannelPosition(&pBuffer[currentPos], pChannelIdEnabledBuff,
                                              channelId, numChannelsEnabled);
            if (syncStatus == 0)
            {
                numGoodSamples++;
                if (numGoodSamples >= requiredGoodSamples)
                {
                    status = ADI_METIC_STATUS_SUCCESS;
                    break;
                }
                numRemainingBytes -= (numChannelsEnabled * 4);
                currentPos += (numChannelsEnabled * 4);
            }
            else
            {
                numRemainingBytes -= 1;
                currentPos += 1;
                numGoodSamples = 0;
                numBytesSkip = currentPos;
            }
        }
        if (numGoodSamples >= requiredGoodSamples)
        {
            *pByteOffset = numBytesSkip;
        }
    }
    return status;
}

ADI_METIC_STATUS adi_metic_WfsUartRxCallback(ADI_METIC_HANDLE hAde)
{
    ADI_METIC_STATUS status = ADI_METIC_STATUS_SUCCESS;
    ADI_METIC_INFO *pInfo = (ADI_METIC_INFO *)hAde;
    if (hAde == NULL)
    {
        status = ADI_METIC_STATUS_NULL_PTR;
    }
    else
    {
        pInfo->wfsData.isWfsRxComplete = 1;
    }

    return status;
}

int32_t CheckChannelPosition(int8_t *pBuffer, uint8_t *pChannelIdEnabledBuff, int32_t startChannel,
                             int32_t numChannelsEnabled)
{
    int32_t status = 1;
    int32_t i;
    int32_t nextChannelIndex = 0;
    int32_t channelId;
    int32_t expectedChannelId = startChannel;

    // Checks all the enabled channels at positions (N+4), (N+8),...
    for (i = 0; i < numChannelsEnabled; i++)
    {
        channelId = pBuffer[nextChannelIndex];
        if ((channelId != expectedChannelId))
        {
            status = 1;
            break;
        }
        status = 0;
        expectedChannelId = GeNextChannelId(pChannelIdEnabledBuff, channelId, numChannelsEnabled);
        nextChannelIndex += 4;
    }

    return status;
}

uint32_t GetBaudRate(uint8_t baudRateBit)
{
    uint32_t baudRate;
    switch (baudRateBit)
    {
    case 0:
        baudRate = 256000;
        break;
    case 1:
        baudRate = 512000;
        break;
    case 2:
        baudRate = 1024000;
        break;
    case 3:
        baudRate = 1536000;
        break;
    case 4:
        baudRate = 2048000;
        break;
    case 5:
        baudRate = 3072000;
        break;
    default:
        baudRate = 3072000;
        break;
    }
    return baudRate;
}

uint16_t GetNumEnabledChannels(uint32_t channelEnable)
{
    int32_t i = 0;
    uint16_t numChannelEnable = 0;
    for (i = 0; i < ADI_METIC_MAX_NUM_CHANNELS; i++)
    {
        if ((channelEnable & (1 << i)) != 0)
        {
            numChannelEnable++;
        }
    }
    return numChannelEnable;
}

void StoreChannelIdEnabled(int32_t channelSelect, uint8_t *pDst)
{
    uint8_t i = 0;
    for (i = 0; i < ADI_METIC_MAX_NUM_CHANNELS; i++)
    {
        if ((channelSelect & (1 << i)) != 0)
        {
            pDst[i] = i;
        }
    }
}

int32_t GeNextChannelId(uint8_t *pChannelIdEnabledBuff, int32_t channelId,
                        int32_t numChannelsEnabled)
{
    int32_t nextChannel = channelId;
    int32_t pos = 0;
    int32_t nextPos = 0;
    int32_t numChannelsChecked = 0;

    // check the next enabled channel by iterating from current channel id through all the
    // channels(channelid - maxchannels).

    while (numChannelsChecked < numChannelsEnabled)
    {
        nextPos = pos + 1;
        if (pos >= numChannelsEnabled)
        {
            pos = 0;
        }
        if (nextPos >= numChannelsEnabled)
        {
            nextPos = 0;
        }
        if (pChannelIdEnabledBuff[pos] == channelId)
        {
            nextChannel = pChannelIdEnabledBuff[nextPos];
            break;
        }
        pos++;
        numChannelsChecked++;
    }

    return nextChannel;
}

/**
 * @}
 */
