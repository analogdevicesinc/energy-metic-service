/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file	    	pulse_count.c
 * @brief       Definitions needed for pulse counter intialisation,storing and getting pulses.
 * @{
 */
/*============= I N C L U D E S =============*/

#include "pulse_count.h"
#include "app_cfg.h"
#include <stdint.h>
#include <string.h>

/*=============  C O D E  =============*/
/**
 * @brief  Function to get remaining free space in the pulse counter buffer.
 * @param[in] pInfo	  - Pointer to structure storing pulse counter data
 *
 */
static int32_t GetSpace(PCNT_INFO *pInfo);
/**
 * @brief  Function to get number of pulses data stored in buffer.
 * @param[in] pInfo	  - Pointer to structure storing pulse counter data
 *
 */
static uint32_t GetNumPulses(PCNT_INFO *pInfo);

void PcntInit(PCNT_INFO *pInfo, uint32_t numPulses, uint32_t *pOutput)
{

    pInfo->pBuffer = pOutput;
    pInfo->readIndex = 0;
    pInfo->writeIndex = 0;
    pInfo->len = numPulses;
}

uint32_t PcntCopyBuffer(PCNT_INFO *pInfo, uint32_t *pOutput)
{
    uint32_t numPulses = GetNumPulses(pInfo);
    uint32_t samplesTillBuffEnd;

    if ((pInfo->readIndex + numPulses) > pInfo->len)
    {
        samplesTillBuffEnd = pInfo->len - pInfo->readIndex;
        memcpy(pOutput, &pInfo->pBuffer[pInfo->readIndex], samplesTillBuffEnd * 4);
        memcpy(&pOutput[samplesTillBuffEnd], &pInfo->pBuffer[0],
               (numPulses - samplesTillBuffEnd) * 4);
        pInfo->readIndex = numPulses - samplesTillBuffEnd;
    }
    else
    {
        memcpy(pOutput, &pInfo->pBuffer[pInfo->readIndex], numPulses * 4);
        pInfo->readIndex += numPulses;
        if (pInfo->readIndex >= pInfo->len)
        {
            pInfo->readIndex = 0;
        }
    }
    return numPulses;
}

void PcntAddToBuffer(PCNT_INFO *pInfo, uint32_t stat, uint32_t time)
{
    int32_t space;

    space = GetSpace(pInfo);
    if (space >= 2)
    {
        pInfo->pBuffer[pInfo->writeIndex++] = stat;
        pInfo->pBuffer[pInfo->writeIndex++] = time;
        if (pInfo->writeIndex >= pInfo->len)
        {
            pInfo->writeIndex = 0;
        }
    }
}

static int32_t GetSpace(PCNT_INFO *pInfo)
{
    int32_t space = (int32_t)pInfo->readIndex - (int32_t)pInfo->writeIndex;
    if (space <= 0)
    {
        space += pInfo->len;
    }

    return space;
}

static uint32_t GetNumPulses(PCNT_INFO *pInfo)
{
    int32_t numPulses = (int32_t)pInfo->writeIndex - (int32_t)pInfo->readIndex;
    if (numPulses < 0)
    {
        numPulses += pInfo->len;
    }

    return (uint32_t)numPulses;
}

/**
 * @}
 */
