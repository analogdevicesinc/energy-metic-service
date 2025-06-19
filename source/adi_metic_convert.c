/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file	    	adi_metic_convert.c
 * @brief       API definitions to convert outputs into proper units.
 * @{
 */
/*============= I N C L U D E S =============*/

#include "adi_metic.h"
#include <stdint.h>

/*============= P R O T O T Y P E S =============*/

/** pi value */
#define PI 3.14159
/** Converts Full scale output format to floating value. */
static float ConvertFixToFloat(int64_t regValue, int32_t fsCode);

/*=============  C O D E  =============*/

void adi_metic_ExtractRegisters(int32_t *pSrc, uint32_t *pIndex, uint32_t numRegisters,
                                int32_t *pDst)
{
    uint32_t i;
    uint32_t index;
    for (i = 0; i < numRegisters; i++)
    {
        index = pIndex[i];
        pDst[i] = pSrc[index];
    }
}

void adi_metic_ConvertPower(ADI_METIC_POWER_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                            ADI_METIC_POWER_OUTPUT *pDst)
{
    uint32_t i;
    int32_t *pPowerSrc = (int32_t *)(pSrc);
    float *pPower = (float *)(pDst);
    for (i = 0; i < numRegisters; i++)
    {
        pPower[i] = ConvertFixToFloat(pPowerSrc[i], ADI_METIC_POWER_FS_CODE);
    }
}

void adi_metic_ConvertPowerFactor(int32_t *pSrc, uint32_t numRegisters, float *pDst)
{
    uint32_t i;
    int32_t *pPowerSrc = (int32_t *)(pSrc);
    float *pPower = (float *)(pDst);
    for (i = 0; i < numRegisters; i++)
    {
        pPower[i] = ConvertFixToFloat(pPowerSrc[i], ADI_METIC_PF_FS_CODE);
    }
}

void adi_metic_ConvertRms(ADI_METIC_RMS_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                          ADI_METIC_RMS_OUTPUT *pDst)
{
    uint32_t i;
    int32_t *pRmsSrc = (int32_t *)(pSrc);
    float *pRms = (float *)(pDst);

    for (i = 0; i < numRegisters; i++)
    {
        pRms[i] = ConvertFixToFloat(pRmsSrc[i], ADI_METIC_RMS_FS_CODE);
    }
}

void adi_metic_ConvertEnergy(ADI_METIC_ENERGY_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                             ADI_METIC_ENERGY_OUTPUT *pDst)
{

    int64_t energy;
    uint32_t i;
    int64_t energyHi;
    int32_t numSample = ADI_METIC_SAMPLING_RATE;
    float *pEnergy = (float *)(pDst);
    int32_t *pEnergySrc = (int32_t *)(pSrc);
    for (i = 0; i < numRegisters / 2; i++)
    {
        energyHi = (int64_t)pEnergySrc[2 * i + 1] << ADI_METIC_ENERGY_HI_POS;
        energy = (energyHi | (int64_t)pEnergySrc[2 * i]);
        pEnergy[i] = ConvertFixToFloat(energy, ADI_METIC_POWER_FS_CODE);
        pEnergy[i] = pEnergy[i] / (float)numSample;
    }
}

void adi_metic_ConvertPeriod(ADI_METIC_PERIOD_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                             ADI_METIC_PERIOD_OUTPUT *pDst)
{
    uint32_t i;
    int32_t format = (ADI_METIC_SAMPLING_RATE * ADI_METIC_PERIOD_FORMAT);
    int32_t *pPeriodSrc = (int32_t *)(pSrc);
    float *pPeriod = (float *)(pDst);

    for (i = 0; i < numRegisters; i++)
    {
        pPeriod[i] = ConvertFixToFloat(pPeriodSrc[i] + 1, format);
    }
}

void adi_metic_ConvertAngle(ADI_METIC_ANGLE_OUTPUT_FIX *pAngleSrc, int32_t *pPeriodSrc,
                            uint32_t numRegisters, ADI_METIC_ANGLE_OUTPUT *pDst)
{
    uint32_t i;
    int32_t *pSrc1 = (int32_t *)(pAngleSrc);
    int32_t *pSrc2 = (int32_t *)(pPeriodSrc);
    float *pAngle = (float *)(pDst);
    for (i = 0; i < numRegisters; i++)
    {
        pAngle[i] = ((float)(2 * PI * pSrc1[i] * ADI_METIC_ANGLE_SCALE)) / ((float)(pSrc2[i] + 1));
    }
}

float ConvertFixToFloat(int64_t regValue, int32_t fsCode)
{
    float out;
    out = (float)regValue / (float)fsCode;
    return out;
}

/**
 * @}
 */
