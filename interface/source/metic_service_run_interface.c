/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_service_run_interface.c
 * @brief       Interface file for extracting outputs using Metrology Service API's.
 * @{
 */

/*============= I N C L U D E S =============*/

#include "adi_cli.h"
#include "adi_evb.h"
#include "adi_metic.h"
#include "app_cfg.h"
#include "metic_output_groups.h"
#include "metic_service_interface.h"
#include <stdint.h>
#include <string.h>

/** Number of angle outputs per cycle */
#define NUM_ANGLE_OUTPUT_PER_CYCLE 9
/** ADC0_STATUSx Mask  */
#define ADC0_ERROR_MASK                                                                            \
    (ADE9178_BITM_ERROR_STATUS_ADC0_STATUS0 | ADE9178_BITM_ERROR_STATUS_ADC0_STATUS1 |             \
     ADE9178_BITM_ERROR_STATUS_ADC0_STATUS2)
/** ADC1_STATUSx Mask  */
#define ADC1_ERROR_MASK                                                                            \
    (ADE9178_BITM_ERROR_STATUS_ADC1_STATUS0 | ADE9178_BITM_ERROR_STATUS_ADC1_STATUS1 |             \
     ADE9178_BITM_ERROR_STATUS_ADC1_STATUS2)
/** ADC2_STATUSx Mask  */
#define ADC2_ERROR_MASK                                                                            \
    (ADE9178_BITM_ERROR_STATUS_ADC2_STATUS0 | ADE9178_BITM_ERROR_STATUS_ADC2_STATUS1 |             \
     ADE9178_BITM_ERROR_STATUS_ADC2_STATUS2)
/** ADC3_STATUSx Mask  */
#define ADC3_ERROR_MASK                                                                            \
    (ADE9178_BITM_ERROR_STATUS_ADC3_STATUS0 | ADE9178_BITM_ERROR_STATUS_ADC3_STATUS1 |             \
     ADE9178_BITM_ERROR_STATUS_ADC3_STATUS2)

/** temporary buffer to store the output index of register values*/
static uint32_t outputIndex[ADI_METIC_MAX_NUM_REGISTERS];
/** temporary buffer for period outputs to calculate angle */
static int32_t periodOutput[NUM_ANGLE_OUTPUT_PER_CYCLE];

static void ExtractAndConvertOutputs(int32_t *pSrc, ADI_METIC_OUTPUT_FIX *pOutputFix,
                                     ADI_METIC_OUTPUT *pOutput);
static void ExtractAndConvertPowerOutput(int32_t *pSrc, ADI_METIC_OUTPUT_FIX *pOutputFix,
                                         ADI_METIC_OUTPUT *pOutput);
static void ExtractAndConvertRmsOutput(int32_t *pSrc, ADI_METIC_RMS_OUTPUT_FIX *pOutputFix,
                                       ADI_METIC_RMS_OUTPUT *pOutput);
static void ExtractAndConvertEnergyOutput(int32_t *pSrc, ADI_METIC_ENERGY_OUTPUT_FIX *pOutputFix,
                                          ADI_METIC_ENERGY_OUTPUT *pOutput);
static void ExtractAndConvertPeriodOutput(int32_t *pSrc, ADI_METIC_PERIOD_OUTPUT_FIX *pOutputFix,
                                          ADI_METIC_PERIOD_OUTPUT *pOutput);
static void ExtractAndConvertAngleOutput(int32_t *pSrc,
                                         ADI_METIC_PERIOD_OUTPUT_FIX *pPeriodOutputFix,
                                         ADI_METIC_ANGLE_OUTPUT_FIX *pAngleOutputFix,
                                         ADI_METIC_ANGLE_OUTPUT *pAngleOutput);
static void ExtractStatusOutput(int32_t *pSrc, ADI_METIC_STATUS_OUTPUT *pAngleOutput);

static int32_t ReadOutputRegisters(METIC_INSTANCE_INFO *pInfo, ADI_METIC_OUTPUT *pOutput);
static ADI_METIC_STATUS ClearAdcStatusRegisters(METIC_INSTANCE_INFO *pInfo, uint8_t device,
                                                int32_t errorRegStatus);

/**
 * @brief Function to get OUTPUT register index from pSrc buffer.
 * @param[in] pSrc 		- User instance
 * @param[in] numRegisters 		- number of registers
 * @param[in] pDst 		- stores index values of input buffer.
 *
 */
static void GetOutputIndex(uint32_t *pSrc, uint32_t numRegisters, uint32_t *pDst);

/**
 * @brief Function to get STATUS register index from pSrc buffer.
 * @param[in] pSrc 		- User instance
 * @param[in] numRegisters 		- number of registers
 * @param[in] pDst 		- stores index values of input buffer.
 *
 */
static void GetStatusOutputIndex(uint32_t *pSrc, uint32_t numRegisters, uint32_t *pDst);

/*=============  C O D E  =============*/

ADI_METIC_STATUS MetIcIfResetIrqStatus(METIC_INSTANCE_INFO *pInfo)
{
    ADI_METIC_STATUS adeStatus = ADI_METIC_STATUS_SUCCESS;
    int32_t status0 = 0x7FFFFFFF;

    // clear all status before starting the measurements.
    pInfo->irqStatus.irq0Ready = 0;
    pInfo->irqStatus.irq0Count = 0;
    pInfo->irqStatus.lastIrqTime = 0;
    // Clear if there are any pending interrupt
    adeStatus = adi_metic_WriteRegister(pInfo->hAde, 0, ADE9178_REG_STATUS0, &status0);

    return adeStatus;
}

int32_t MetIcIfReadMetrologyParameters(ADI_CLI_HANDLE hCli, METIC_INSTANCE_INFO *pInfo,
                                       ADI_METIC_OUTPUT *pOutput)
{
    int32_t status0;

    int32_t status = 1;
    uint32_t maxTime = 0;
    uint32_t currTime;
    ADI_METIC_STATUS adeStatus = ADI_METIC_STATUS_SUCCESS;
    uint32_t freeSpace;
    if (pInfo->irqStatus.irq0Ready == 1)
    {
        status = 0;
        pInfo->irqStatus.lastIrqTime = EvbGetTime();
        pInfo->irqStatus.irq0Ready = 0;
        adeStatus = adi_metic_ReadRegister(pInfo->hAde, 0, ADE9178_REG_STATUS0, 1, &status0);
        if ((adeStatus == ADI_METIC_STATUS_SUCCESS))
        {
            adeStatus = adi_metic_WriteRegister(pInfo->hAde, 0, ADE9178_REG_STATUS0, &status0);
        }
        if ((adeStatus == ADI_METIC_STATUS_SUCCESS))
        {
            if (pInfo->enableRegisterRead == 1)
            {
                adi_cli_GetFreeMessageSpace(hCli, &freeSpace);
                if (freeSpace > MAX_MSG_STORAGE_SIZE_PER_CYCLE)
                {
                    ReadOutputRegisters(pInfo, pOutput);
                }
                else
                {
                    status = 10;
                }
            }
        }
        else
        {
            status = 3;
        }
    }
    else
    {
        currTime = EvbGetTime();
        if (currTime < pInfo->irqStatus.lastIrqTime)
        {
            maxTime = EvbGetMaxTime();
        }
        if (((currTime >= pInfo->irqStatus.lastIrqTime) &&
             (currTime - pInfo->irqStatus.lastIrqTime) > APP_CFG_IRQ_TIMEOUT) ||
            ((currTime < pInfo->irqStatus.lastIrqTime) &&
             ((maxTime - pInfo->irqStatus.lastIrqTime + currTime) > APP_CFG_IRQ_TIMEOUT)))
        {
            status = 5;
        }
    }

    return status;
}

ADI_METIC_STATUS MetIcIfGetErrorStatus(METIC_INSTANCE_INFO *pInfo, int32_t *pErrorRegStatus)
{
    ADI_METIC_STATUS adeStatus = 0;
    int32_t statusbitClear = ADE9178_BITM_STATUS1_ERROR;
    int32_t i;
    int32_t errorValue = 0;
    *pErrorRegStatus = 0;

    if (pInfo->irqStatus.irq1Ready == 1) // error has occured, read error status.
    {
        pInfo->irqStatus.irq1Ready = 0;
        adeStatus =
            adi_metic_ReadRegister(pInfo->hAde, 0, ADE9178_REG_ERROR_STATUS, 1, pErrorRegStatus);
        errorValue = *pErrorRegStatus;
    }

    if ((errorValue != 0) && (adeStatus == 0))
    {
        adeStatus =
            adi_metic_WriteRegister(pInfo->hAde, 0, ADE9178_REG_ERROR_STATUS, pErrorRegStatus);
        adeStatus = MetIcIfClearAdcStatusError(pInfo, errorValue);
        adeStatus = adi_metic_WriteRegister(pInfo->hAde, 0, ADE9178_REG_STATUS1, &statusbitClear);
        for (i = 0; i < ERROR_COUNT_BUFFER_SIZE; i++)
        {
            if (((errorValue >> i) & 0x1) == 1)
            {
                pInfo->errorStatusCount[i] += 1;
            }
        }
    }

    return adeStatus;
}

ADI_METIC_STATUS MetIcIfCollectSamples(METIC_INSTANCE_INFO *pInfo, int32_t config)
{
    ADI_METIC_STATUS status = 0;
    pInfo->isWfsRxComplete = 0;
    status = adi_metic_WfsConfigureRx(pInfo->hAde, config);
    if (status == 0)
    {
        status = adi_metic_WfsStartRx(pInfo->hAde, WFS_BUFFER_SIZE * sizeof(int32_t),
                                      (uint8_t *)&pInfo->wfsBuffer[0]);
    }

    return status;
}

ADI_METIC_STATUS MetIcIfClearAdcStatusError(METIC_INSTANCE_INFO *pInfo, int32_t errorRegStatus)
{
    ADI_METIC_STATUS adeStatus = 0;

    if ((errorRegStatus & ADC0_ERROR_MASK) != 0)
    {
        // Issue CRC_FORCE and clear CRC_DONE for STATUS0 Errors
        ClearAdcStatusRegisters(pInfo, 1, errorRegStatus);
    }
    if ((errorRegStatus & ADC1_ERROR_MASK) != 0)
    {
        ClearAdcStatusRegisters(pInfo, 2, errorRegStatus);
    }
    if ((errorRegStatus & ADC2_ERROR_MASK) != 0)
    {
        ClearAdcStatusRegisters(pInfo, 3, errorRegStatus);
    }
    if ((errorRegStatus & ADC3_ERROR_MASK) != 0)
    {
        ClearAdcStatusRegisters(pInfo, 4, errorRegStatus);
    }

    return adeStatus;
}

static ADI_METIC_STATUS ClearAdcStatusRegisters(METIC_INSTANCE_INFO *pInfo, uint8_t device,
                                                int32_t errorRegStatus)
{
    int32_t statusBit = 0;
    int32_t crcForceValue = 0x1;
    int32_t crcDoneValue = 0x2;
    ADI_METIC_STATUS adeStatus = 0;
    int32_t status0Mask = 0;
    int32_t status1Mask = 0;
    int32_t status2Mask = 0;
    switch (device)
    {
    case 1:
        status0Mask = ADE9178_BITM_ERROR_STATUS_ADC0_STATUS0;
        status1Mask = ADE9178_BITM_ERROR_STATUS_ADC0_STATUS1;
        status2Mask = ADE9178_BITM_ERROR_STATUS_ADC0_STATUS2;
        break;
    case 2:
        status0Mask = ADE9178_BITM_ERROR_STATUS_ADC1_STATUS0;
        status1Mask = ADE9178_BITM_ERROR_STATUS_ADC1_STATUS1;
        status2Mask = ADE9178_BITM_ERROR_STATUS_ADC1_STATUS2;
        break;
    case 3:
        status0Mask = ADE9178_BITM_ERROR_STATUS_ADC2_STATUS0;
        status1Mask = ADE9178_BITM_ERROR_STATUS_ADC2_STATUS1;
        status2Mask = ADE9178_BITM_ERROR_STATUS_ADC2_STATUS2;
        break;
    case 4:
        status0Mask = ADE9178_BITM_ERROR_STATUS_ADC3_STATUS0;
        status1Mask = ADE9178_BITM_ERROR_STATUS_ADC3_STATUS1;
        status2Mask = ADE9178_BITM_ERROR_STATUS_ADC3_STATUS2;
        break;
    }
    if ((errorRegStatus & status0Mask) != 0)
    {

        adeStatus = adi_metic_WriteRegister(pInfo->hAde, device, 0x25, &crcForceValue);
        adeStatus = adi_metic_WriteRegister(pInfo->hAde, device, 0x25, &crcDoneValue);
        adeStatus = adi_metic_ReadRegister(pInfo->hAde, device, 0x20, 1, &statusBit);
        statusBit = (statusBit >> 8) & 0xFF;
        adeStatus = adi_metic_WriteRegister(pInfo->hAde, device, 0x20, &statusBit);
    }
    if ((errorRegStatus & status1Mask) != 0)
    {
        adeStatus = adi_metic_ReadRegister(pInfo->hAde, device, 0x21, 1, &statusBit);
        statusBit = (statusBit >> 8) & 0xFF;
        adeStatus = adi_metic_WriteRegister(pInfo->hAde, device, 0x21, &statusBit);
    }
    if ((errorRegStatus & status2Mask) != 0)
    {
        adeStatus = adi_metic_ReadRegister(pInfo->hAde, device, 0x22, 1, &statusBit);
        statusBit = (statusBit >> 8) & 0xFF;
        adeStatus = adi_metic_WriteRegister(pInfo->hAde, device, 0x22, &statusBit);
    }
    return adeStatus;
}

int32_t ReadOutputRegisters(METIC_INSTANCE_INFO *pInfo, ADI_METIC_OUTPUT *pOutput)
{
    uint32_t numRegisters;
    int32_t status = 0;
    uint32_t numStatusRegisters;
    ADI_METIC_STATUS adeStatus = ADI_METIC_STATUS_SUCCESS;
    int32_t *pRegOutput = &pInfo->regBuffer[0];
    int32_t *pRegStatusOutput = &pInfo->regStatusBuffer[0];

    numRegisters = ADE9178_REG_COM_PERIOD - ADE9178_REG_AVRMS + 1;
    adeStatus = adi_metic_ReadRegister(pInfo->hAde, 0, ADE9178_REG_AVRMS, numRegisters, pRegOutput);
    if (adeStatus == ADI_METIC_STATUS_SUCCESS)
    {

        numStatusRegisters = ADE9178_REG_ERROR_STATUS - ADE9178_REG_STATUS0 + 1;

        adeStatus = adi_metic_ReadRegister(pInfo->hAde, 0, ADE9178_REG_STATUS0, numStatusRegisters,
                                           pRegStatusOutput);
    }

    if (adeStatus == ADI_METIC_STATUS_SUCCESS)
    {
        ExtractAndConvertOutputs(pRegOutput, &pInfo->outputFix, pOutput);
        ExtractStatusOutput(pRegStatusOutput, &pOutput->statusOut);
    }

    return status;
}

void ExtractAndConvertOutputs(int32_t *pSrc, ADI_METIC_OUTPUT_FIX *pOutputFix,
                              ADI_METIC_OUTPUT *pOutput)
{
    ExtractAndConvertPowerOutput(pSrc, pOutputFix, pOutput);
    ExtractAndConvertRmsOutput(pSrc, &pOutputFix->rmsOut[0], &pOutput->rmsOut[0]);
    ExtractAndConvertEnergyOutput(pSrc, &pOutputFix->energyOut[0], &pOutput->energyOut[0]);
    ExtractAndConvertPeriodOutput(pSrc, &pOutputFix->periodOut, &pOutput->periodOut);
    ExtractAndConvertAngleOutput(pSrc, &pOutputFix->periodOut, &pOutputFix->angleOut,
                                 &pOutput->angleOut);
}

void ExtractAndConvertPowerOutput(int32_t *pSrc, ADI_METIC_OUTPUT_FIX *pOutputFix,
                                  ADI_METIC_OUTPUT *pOutput)
{
    ADI_METIC_POWER_OUTPUT_FIX *pPowerOutFix = &pOutputFix->powerOut[0];
    ADI_METIC_POWER_OUTPUT *pPowerOut = &pOutput->powerOut[0];
    uint32_t numRegisters = sizeof(powerRegisters) / sizeof(powerRegisters[0]);

    GetOutputIndex(&powerRegisters[0], numRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numRegisters, (int32_t *)pPowerOutFix);
    adi_metic_ConvertPower(pPowerOutFix, numRegisters, pPowerOut);

    numRegisters = sizeof(powerFactorRegisters) / sizeof(powerFactorRegisters[0]);
    GetOutputIndex(&powerFactorRegisters[0], numRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numRegisters,
                               (int32_t *)&pOutputFix->powerFactor[0]);
    adi_metic_ConvertPowerFactor(&pOutputFix->powerFactor[0], numRegisters,
                                 &pOutput->powerFactor[0]);
}

void ExtractAndConvertRmsOutput(int32_t *pSrc, ADI_METIC_RMS_OUTPUT_FIX *pOutputFix,
                                ADI_METIC_RMS_OUTPUT *pOutput)
{
    uint32_t numRegisters = sizeof(rmsRegisters) / sizeof(rmsRegisters[0]);
    GetOutputIndex(&rmsRegisters[0], numRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numRegisters, (int32_t *)pOutputFix);
    adi_metic_ConvertRms(pOutputFix, numRegisters, pOutput);
}
void ExtractAndConvertEnergyOutput(int32_t *pSrc, ADI_METIC_ENERGY_OUTPUT_FIX *pOutputFix,
                                   ADI_METIC_ENERGY_OUTPUT *pOutput)
{
    uint32_t numRegisters = sizeof(energyRegisters) / sizeof(energyRegisters[0]);

    GetOutputIndex(&energyRegisters[0], numRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numRegisters, (int32_t *)pOutputFix);
    adi_metic_ConvertEnergy(pOutputFix, numRegisters, pOutput);
}
void ExtractAndConvertPeriodOutput(int32_t *pSrc, ADI_METIC_PERIOD_OUTPUT_FIX *pOutputFix,
                                   ADI_METIC_PERIOD_OUTPUT *pOutput)
{
    uint32_t numRegisters = sizeof(periodRegisters) / sizeof(periodRegisters[0]);
    GetOutputIndex(&periodRegisters[0], numRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numRegisters, (int32_t *)pOutputFix);
    adi_metic_ConvertPeriod(pOutputFix, numRegisters, pOutput);
}
void ExtractAndConvertAngleOutput(int32_t *pSrc, ADI_METIC_PERIOD_OUTPUT_FIX *pPeriodOutputFix,
                                  ADI_METIC_ANGLE_OUTPUT_FIX *pAngleOutputFix,
                                  ADI_METIC_ANGLE_OUTPUT *pAngleOutput)
{
    uint32_t numAngleRegisters = sizeof(angleRegisters) / sizeof(angleRegisters[0]);
    uint32_t numPeriodRegisters = sizeof(periodRegisters) / sizeof(periodRegisters[0]);
    GetOutputIndex(&angleRegisters[0], numAngleRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numAngleRegisters,
                               (int32_t *)pAngleOutputFix);
    GetOutputIndex(&periodRegisters[0], numPeriodRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numPeriodRegisters,
                               (int32_t *)pPeriodOutputFix);
    // angle conversion is dependent on the period registers. period outputs are [APERIOD, BPERIOD,
    // CPERIOD,APERIOD, BPERIOD, CPERIOD,APERIOD, BPERIOD, CPERIOD]
    memcpy(periodOutput, pPeriodOutputFix, 3 * sizeof(uint32_t));
    memcpy(&periodOutput[3], pPeriodOutputFix, 3 * sizeof(uint32_t));
    memcpy(&periodOutput[6], pPeriodOutputFix, 3 * sizeof(uint32_t));
    adi_metic_ConvertAngle(pAngleOutputFix, periodOutput, numAngleRegisters, pAngleOutput);
}

void ExtractStatusOutput(int32_t *pSrc, ADI_METIC_STATUS_OUTPUT *pOutput)
{
    uint32_t numRegisters = sizeof(statusRegisters) / sizeof(statusRegisters[0]);
    GetStatusOutputIndex(&statusRegisters[0], numRegisters, &outputIndex[0]);
    adi_metic_ExtractRegisters(pSrc, &outputIndex[0], numRegisters, (int32_t *)pOutput);
}

void GetOutputIndex(uint32_t *pSrc, uint32_t numRegisters, uint32_t *pDst)
{
    uint32_t i;
    uint32_t index;
    uint32_t baseIndex = ADE9178_REG_AVRMS;
    for (i = 0; i < numRegisters; i++)
    {
        index = pSrc[i] - baseIndex;
        pDst[i] = index;
    }
}

void GetStatusOutputIndex(uint32_t *pSrc, uint32_t numRegisters, uint32_t *pDst)
{
    uint32_t i;
    uint32_t index;
    uint32_t baseIndex = ADE9178_REG_STATUS0;
    for (i = 0; i < numRegisters; i++)
    {
        index = pSrc[i] - baseIndex;
        pDst[i] = index;
    }
}

/**
 * @}
 */
