/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @brief       Functions to display on cli
 * @{
 */

#include "example_display.h"
#include "adi_cli.h"
#include "adi_metic_status.h"
#include "metic_service_interface.h"
#include <string.h>

#define CHANNEL_ID 0

static void DisplayCalCoeffs(ADE_CONFIG_REG *pConfig);
static void DisplayPowerCalCoeffs(ADE_CONFIG_REG *pConfig);
static void DisplayCommReg(ADE_CONFIG_REG *pConfig);
static void DisplayStatusReg(ADE_CONFIG_REG *pConfig);
static void DisplayChannelRmsOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount,
                                    ADI_METIC_RMS_OUTPUT *pOutput);
static void DisplayPowerAndEnergyOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount,
                                        ADI_METIC_OUTPUT *pOutput);
static void DisplayPeriodOutput(uint32_t irqCount, ADI_METIC_PERIOD_OUTPUT *pOutput);
static void DisplayAngleOutput(uint32_t irqCount, ADI_METIC_ANGLE_OUTPUT *pAngleOutput);
static void DisplayStatusOutput(uint32_t irqCount, ADI_METIC_STATUS_OUTPUT *pAngleOutput);
static void DisplayAuxOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount,
                             ADI_METIC_RMS_OUTPUT *pOutput);
/** List of available channels*/
static char *channel[] = {"AV",   "AI",   "BV",   "BI",   "CV",   "CI",
                          "AUX0", "AUX1", "AUX2", "AUX3", "AUX4", "AUX5"};
static char *powerChannel[] = {"A", "B", "C"};

#define MAX_MSG_STORAGE_SIZE_PER_CYCLE 3806

void InitDisplayConfig(ADE_DISPLAY_CONFIG *pConfig)
{

    pConfig->enableRmsOutput = true;
    pConfig->enableRmsOneOutput = true;
    pConfig->enableRmsHalfOutput = true;
    pConfig->enablePowerOutput = true;
    pConfig->enableEnergyOutput = true;
    pConfig->enablePeriodOutput = true;
    pConfig->enableAngleOutput = true;
    pConfig->enableEventRmsOneOutput = false;
    pConfig->enableEventRmsHalfOutput = false;
    pConfig->enableStatusOutput = true;
    pConfig->enableErrorStatusOutput = false;
    pConfig->voltageScale = 707;
    pConfig->currentScale = 44.188f;
    pConfig->auxScale = 707;
}

void DisplayNvmReg(ADE_CONFIG_REG *pConfig)
{
    DisplayCalCoeffs(pConfig);
    DisplayPowerCalCoeffs(pConfig);
    DisplayCommReg(pConfig);
    DisplayStatusReg(pConfig);
}
void DisplayCalCoeffs(ADE_CONFIG_REG *pConfig)
{
    uint8_t i;
    char *channelOrder[] = {"AV",   "BV",   "CV",   "AI",   "BI",   "CI",
                            "AUX0", "AUX1", "AUX2", "AUX3", "AUX4", "AUX5"};
    for (i = 0; i < 6; i++)
    {
        INFO_MSG("%sGAIN         (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].gain.address, pConfig->channelConfig[i].gain.value)
        INFO_MSG("%sOS           (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].offset.address, pConfig->channelConfig[i].offset.value)
        INFO_MSG("%sPHCAL        (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].phcal.address, pConfig->channelConfig[i].phcal.value)
        INFO_MSG("%sRMSOS        (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].rmsoffset.address,
                 pConfig->channelConfig[i].rmsoffset.value)
        INFO_MSG("%sRMSHALFOS    (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].rmshalfoffset.address,
                 pConfig->channelConfig[i].rmshalfoffset.value)
        INFO_MSG("%sRMSONEOS     (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].rmsoneoffset.address,
                 pConfig->channelConfig[i].rmsoneoffset.value)
    }
    for (i = 6; i < 12; i++)
    {
        INFO_MSG("%sGAIN       (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].gain.address, pConfig->channelConfig[i].gain.value)
        INFO_MSG("%sOS         (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].offset.address, pConfig->channelConfig[i].offset.value)
        INFO_MSG("%sPHCAL      (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].phcal.address, pConfig->channelConfig[i].phcal.value)
        INFO_MSG("%sRMSOS      (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].rmsoffset.address,
                 pConfig->channelConfig[i].rmsoffset.value)
        INFO_MSG("%sRMSHALFOS  (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].rmshalfoffset.address,
                 pConfig->channelConfig[i].rmshalfoffset.value)
        INFO_MSG("%sRMSONEOS   (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->channelConfig[i].rmsoneoffset.address,
                 pConfig->channelConfig[i].rmsoneoffset.value)
    }
}

void DisplayPowerCalCoeffs(ADE_CONFIG_REG *pConfig)
{
    uint8_t i;
    char *channelOrder[] = {"A", "B", "C"};
    for (i = 0; i < ADI_METIC_MAX_NUM_POWER_CHANNELS; i++)
    {
        INFO_MSG("%sPGAIN         (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->powerConfig[i].gain.address, pConfig->powerConfig[i].gain.value)
        INFO_MSG("%sWATTOS        (0x%x)  = 0x%x", channelOrder[i],
                 pConfig->powerConfig[i].offset.address, pConfig->powerConfig[i].offset.value)
    }
}

void DisplayCommReg(ADE_CONFIG_REG *pConfig)
{
    INFO_MSG("CONFIG0        (0x%x)  = 0x%x", pConfig->commConfig.config0.address,
             pConfig->commConfig.config0.value)
    INFO_MSG("CF1_CONFIG     (0x%x)  = 0x%x", pConfig->commConfig.cf1Config.address,
             pConfig->commConfig.cf1Config.value)
    INFO_MSG("CF2_CONFIG     (0x%x)  = 0x%x", pConfig->commConfig.cf2Config.address,
             pConfig->commConfig.cf2Config.value)
    INFO_MSG("VNOM           (0x%x)  = 0x%x", pConfig->commConfig.vNom.address,
             pConfig->commConfig.vNom.value)
    INFO_MSG("USER_PERIOD    (0x%x)  = 0x%x", pConfig->commConfig.userPeriod.address,
             pConfig->commConfig.userPeriod.value)
    INFO_MSG("CF1_THR        (0x%x)  = 0x%x", pConfig->commConfig.cf1Thrsh.address,
             pConfig->commConfig.cf1Thrsh.value)
    INFO_MSG("CF2_THR        (0x%x)  = 0x%x", pConfig->commConfig.cf2Thrsh.address,
             pConfig->commConfig.cf2Thrsh.value)
    INFO_MSG("ZXTHRSH        (0x%x)  = 0x%x", pConfig->commConfig.zxThrsh.address,
             pConfig->commConfig.zxThrsh.value)
    INFO_MSG("ZX_LP_SEL      (0x%x)  = 0x%x", pConfig->commConfig.zxLpSel.address,
             pConfig->commConfig.zxLpSel.value)
    INFO_MSG("ACT_NL_LVL     (0x%x)  = 0x%x", pConfig->commConfig.actPwrNoLoadLvl.address,
             pConfig->commConfig.actPwrNoLoadLvl.value)
    INFO_MSG("APP_NL_LVL     (0x%x)  = 0x%x", pConfig->commConfig.appPwrNoLoadLvl.address,
             pConfig->commConfig.appPwrNoLoadLvl.value)
    INFO_MSG("EP_CFG         (0x%x)  = 0x%x", pConfig->commConfig.epConfig.address,
             pConfig->commConfig.epConfig.value)
    INFO_MSG("EGY_TIME       (0x%x)  = 0x%x", pConfig->commConfig.energyTime.address,
             pConfig->commConfig.energyTime.value)
    INFO_MSG("CF_LTMR        (0x%x)  = 0x%x", pConfig->cfLtmr.address, pConfig->cfLtmr.value)
    INFO_MSG("ADC_CONFIG     (0x%x)  = 0x%x", pConfig->adcConfig.address, pConfig->adcConfig.value)
    INFO_MSG("ADC_REDIRECT0  (0x%x)  = 0x%x", pConfig->adcRedirect0.address,
             pConfig->adcRedirect0.value)
    INFO_MSG("ADC_REDIRECT1  (0x%x)  = 0x%x", pConfig->adcRedirect1.address,
             pConfig->adcRedirect1.value)
}
void DisplayStatusReg(ADE_CONFIG_REG *pConfig)
{
    INFO_MSG("MASK0          (0x%x) = 0x%x", pConfig->statusConfig.mask0.address,
             pConfig->statusConfig.mask0.value)
    INFO_MSG("MASK1          (0x%x) = 0x%x", pConfig->statusConfig.mask1.address,
             pConfig->statusConfig.mask1.value)
    INFO_MSG("MASK2          (0x%x) = 0x%x", pConfig->statusConfig.mask2.address,
             pConfig->statusConfig.mask2.value)
    INFO_MSG("MASK3          (0x%x) = 0x%x", pConfig->statusConfig.mask3.address,
             pConfig->statusConfig.mask3.value)
    INFO_MSG("ERROR_MASK     (0x%x) = 0x%x", pConfig->statusConfig.errorMask.address,
             pConfig->statusConfig.errorMask.value)
}

void DisplayOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount, ADI_METIC_OUTPUT *pOutput)
{
    if (pDisplay->enableRmsOutput || pDisplay->enableRmsOneOutput ||
        pDisplay->enableRmsHalfOutput || pDisplay->enableEventRmsOneOutput ||
        pDisplay->enableEventRmsHalfOutput)
    {
        DisplayChannelRmsOutput(pDisplay, irqCount, &pOutput->rmsOut[0]);
    }
    if (pDisplay->enablePowerOutput || pDisplay->enableEnergyOutput)
    {
        DisplayPowerAndEnergyOutput(pDisplay, irqCount, pOutput);
    }
    if (pDisplay->enableAngleOutput)
    {
        DisplayAngleOutput(irqCount, &pOutput->angleOut);
    }
    if (pDisplay->enablePeriodOutput)
    {
        DisplayPeriodOutput(irqCount, &pOutput->periodOut);
    }
    if (pDisplay->enableStatusOutput)
    {
        DisplayStatusOutput(irqCount, &pOutput->statusOut);
    }
}

void DisplayChannelRmsOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount,
                             ADI_METIC_RMS_OUTPUT *pOutput)
{
    int16_t i;
    float scale;

    for (i = 0; i < 6; i++)
    {
        if (i % 2 == 0)
        {
            scale = pDisplay->voltageScale;
        }
        else
        {
            scale = pDisplay->currentScale;
        }
        if (pDisplay->enableRmsOutput)
        {
            INFO_MSG("%d, %sRMS           = %f", irqCount, channel[i],
                     (double)(pOutput[i].filteredRms * scale))
        }
        if (pDisplay->enableRmsHalfOutput)
        {
            INFO_MSG("%d, %sRMSHALF       = %f", irqCount, channel[i],
                     (double)(pOutput[i].rmsHalfCycle * scale))
        }
        if (pDisplay->enableRmsOneOutput)
        {
            INFO_MSG("%d, %sRMSONE        = %f", irqCount, channel[i],
                     (double)(pOutput[i].rmsOneCycle * scale))
        }
        if (pDisplay->enableEventRmsHalfOutput)
        {
            INFO_MSG("%d, %sDIPHALF       = %f", irqCount, channel[i],
                     (double)(pOutput[i].dipHalf * scale))
        }
        if (pDisplay->enableEventRmsOneOutput)
        {
            INFO_MSG("%d, %sDIPONE        = %f", irqCount, channel[i],
                     (double)(pOutput[i].dipOne * scale))
        }
        if (pDisplay->enableEventRmsHalfOutput)
        {
            INFO_MSG("%d, %sSWELLHALF     = %f", irqCount, channel[i],
                     (double)(pOutput[i].swellHalf * scale))
        }
        if (pDisplay->enableEventRmsOneOutput)
        {
            INFO_MSG("%d, %sSWELLONE      = %f", irqCount, channel[i],
                     (double)(pOutput[i].swellOne * scale))
        }
    }
    DisplayAuxOutput(pDisplay, irqCount, pOutput);
}

void DisplayAuxOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount,
                      ADI_METIC_RMS_OUTPUT *pOutput)
{
    int16_t i;
    float scale;
    scale = pDisplay->auxScale;
    // For the formatting, seperated the AUX channel display message.
    for (i = 6; i < ADI_METIC_MAX_NUM_CHANNELS; i++)
    {
        if (pDisplay->enableRmsOutput)
        {
            INFO_MSG("%d, %sRMS         = %f", irqCount, channel[i],
                     (double)(pOutput[i].filteredRms * scale))
        }
        if (pDisplay->enableRmsHalfOutput)
        {
            INFO_MSG("%d, %sRMSHALF     = %f", irqCount, channel[i],
                     (double)(pOutput[i].rmsHalfCycle * scale))
        }
        if (pDisplay->enableRmsOneOutput)
        {
            INFO_MSG("%d, %sRMSONE      = %f", irqCount, channel[i],
                     (double)(pOutput[i].rmsOneCycle * scale))
        }
        if (pDisplay->enableEventRmsHalfOutput)
        {
            INFO_MSG("%d, %sDIPHALF     = %f", irqCount, channel[i],
                     (double)(pOutput[i].dipHalf * scale))
        }
        if (pDisplay->enableEventRmsOneOutput)
        {
            INFO_MSG("%d, %sDIPONE      = %f", irqCount, channel[i],
                     (double)(pOutput[i].dipOne * scale))
        }
        if (pDisplay->enableEventRmsHalfOutput)
        {
            INFO_MSG("%d, %sSWELLHALF   = %f", irqCount, channel[i],
                     (double)(pOutput[i].swellHalf * scale))
        }
        if (pDisplay->enableEventRmsOneOutput)
        {
            INFO_MSG("%d, %sSWELLONE    = %f", irqCount, channel[i],
                     (double)(pOutput[i].swellOne * scale))
        }
    }
}
void DisplayPowerAndEnergyOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount,
                                 ADI_METIC_OUTPUT *pOutput)
{
    int16_t i;
    float powerScale;
    powerScale = pDisplay->voltageScale * pDisplay->currentScale;

    for (i = 0; i < ADI_METIC_MAX_NUM_POWER_CHANNELS; i++)
    {
        if (pDisplay->enablePowerOutput)
        {

            INFO_MSG("%d, %sWATT           = %f", irqCount, powerChannel[i],
                     (double)(pOutput->powerOut[i].activePower * powerScale))
            INFO_MSG("%d, %sVA             = %f", irqCount, powerChannel[i],
                     (double)(pOutput->powerOut[i].apparentPower * powerScale))
            INFO_MSG("%d, %sPF             = %f", irqCount, powerChannel[i],
                     (double)(pOutput->powerFactor[i]))
        }
        if (pDisplay->enableEnergyOutput)
        {
            INFO_MSG("%d, %sWATTHR_POS     = %f", irqCount, powerChannel[i],
                     (double)(pOutput->energyOut[i].posActEgy * powerScale))
            INFO_MSG("%d, %sWATTHR_NEG     = %f", irqCount, powerChannel[i],
                     (double)(pOutput->energyOut[i].negActEgy * powerScale))
            INFO_MSG("%d, %sWATTHR_SIGNED  = %f", irqCount, powerChannel[i],
                     (double)(pOutput->energyOut[i].signActEgy * powerScale))
            INFO_MSG("%d, %sVAHR           = %f", irqCount, powerChannel[i],
                     (double)(pOutput->energyOut[i].apparentEgy * powerScale))
        }
    }
}

void DisplayAngleOutput(uint32_t irqCount, ADI_METIC_ANGLE_OUTPUT *pOutput)
{

    INFO_MSG("%d, ANGL_AV_BV      = %f", irqCount, (double)pOutput->angl_av_bv)
    INFO_MSG("%d, ANGL_BV_CV      = %f", irqCount, (double)pOutput->angl_bv_cv)
    INFO_MSG("%d, ANGL_AV_CV      = %f", irqCount, (double)pOutput->angl_av_cv)
    INFO_MSG("%d, ANGL_AV_AI      = %f", irqCount, (double)pOutput->angl_av_ai)
    INFO_MSG("%d, ANGL_BV_BI      = %f", irqCount, (double)pOutput->angl_bv_bi)
    INFO_MSG("%d, ANGL_CV_CI      = %f", irqCount, (double)pOutput->angl_cv_ci)
    INFO_MSG("%d, ANGL_AI_BI      = %f", irqCount, (double)pOutput->angl_ai_bi)
    INFO_MSG("%d, ANGL_BI_CI      = %f", irqCount, (double)pOutput->angl_bi_ci)
    INFO_MSG("%d, ANGL_AI_CI      = %f", irqCount, (double)pOutput->angl_ai_ci)
}

void DisplayPeriodOutput(uint32_t irqCount, ADI_METIC_PERIOD_OUTPUT *pOutput)
{

    INFO_MSG("%d, APERIOD         = %f", irqCount, (double)pOutput->aPeriod)
    INFO_MSG("%d, BPERIOD         = %f", irqCount, (double)pOutput->bPeriod)
    INFO_MSG("%d, CPERIOD         = %f", irqCount, (double)pOutput->cPeriod)
    INFO_MSG("%d, COM_PERIOD      = %f", irqCount, (double)pOutput->comPeriod)
}

void DisplayStatusOutput(uint32_t irqCount, ADI_METIC_STATUS_OUTPUT *pOutput)
{

    INFO_MSG("%d, STATUS0         = 0x%x", irqCount, pOutput->status0)
    INFO_MSG("%d, STATUS1         = 0x%x", irqCount, pOutput->status1)
    INFO_MSG("%d, STATUS2         = 0x%x", irqCount, pOutput->status2)
    INFO_MSG("%d, STATUS3         = 0x%x", irqCount, pOutput->status3)
    INFO_MSG("%d, ERROR_STATUS    = 0x%x", irqCount, pOutput->errorStatus)
}

void DisplayWaveformOutput(void)
{
    int32_t i, j;
    uint32_t freeSpace;
    int32_t numChannels;
    int32_t byteOffset = 0;
    int32_t sample;
    int32_t numSamples;
    int32_t numBytes;
    ADI_METIC_STATUS syncStatus = 0;
    METIC_INSTANCE_INFO *pInfo = GetAdeInstance();
    int32_t *pWaveformData = &(pInfo->wfsBuffer[0]);
    int8_t *pBuffer;
#if ENABLE_ALL_CHANNELS == 1
    static char *channelOrder[] = {"AV",   "AI",   "BV",   "BI",   "CV",   "CI",
                                   "AUX0", "AUX1", "AUX2", "AUX3", "AUX4", "AUX5"};
    numChannels = 12;
#else

    static char *channelOrder[] = {"AV", "AI", "BV", "BI", "CV", "CI"};
    numChannels = 6;
#endif
    if (pInfo->isWfsRxComplete == 1)
    {
        numBytes = WFS_BUFFER_SIZE * sizeof(int32_t);
        // Finds channel offset from where proper data starts.
        syncStatus = adi_metic_FindChannelOffset(pInfo->hAde, (int8_t *)pWaveformData, numBytes,
                                                 CHANNEL_ID, &byteOffset);
        numSamples = (numBytes - byteOffset) / 4;
        if (syncStatus == ADI_METIC_STATUS_SUCCESS)
        {
            pBuffer = (int8_t *)&pWaveformData[0];
            memcpy(&pBuffer[0], &pBuffer[byteOffset], (uint32_t)(numBytes - byteOffset));
            if (numSamples >= WFS_NUM_SAMPLES)
            {
                numSamples = WFS_NUM_SAMPLES;
            }
            INFO_MSG("")
            for (i = 0; i < numChannels; i++)
            {
                INFO_MSG_RAW("%s,", channelOrder[i])
            }
            INFO_MSG("")
            for (i = 0; i < numSamples; i = i + numChannels)
            {
                if (i + numChannels <= numSamples)
                {
                    for (j = 0; j < numChannels; j++)
                    {
                        sample = pWaveformData[i + j] >> 8;
                        INFO_MSG_RAW("%d,", sample)
                    }
                    INFO_MSG("")
                }
                do
                {
                    adi_cli_FlushMessages();
                    freeSpace = adi_cli_GetFreeMessageSpace();
                } while (freeSpace < MAX_MSG_STORAGE_SIZE_PER_CYCLE);
            }
        }
        else
        {
            INFO_MSG("waveform samples are not syncronised")
        }
    }
    else
    {
        INFO_MSG("wfs buffer is not full")
    }
}

void DisplayConfigValue(uint16_t configChoice)
{
    METIC_EXAMPLE_CONFIG *pConfig = GetExampleConfig();
    switch (configChoice)
    {
    case METIC_EXAMPLE_CONFIG_VOLTAGE_SCALE:
        INFO_MSG("voltage scale: %f", (double)pConfig->displayConfig.voltageScale)
        break;
    case METIC_EXAMPLE_CONFIG_CURRENT_SCALE:
        INFO_MSG("current scale: %f", (double)pConfig->displayConfig.currentScale)
        break;
    case METIC_EXAMPLE_CONFIG_AUX_SCALE:
        INFO_MSG("aux scale: %f", (double)pConfig->displayConfig.auxScale)
        break;
    default:
        WARN_MSG("Invalid config choice")
        break;
    }
}

void DisplayBurstRegisters(uint32_t addr, uint32_t numRegisters, int32_t *pRegData)
{
    typedef struct
    {
        uint32_t start;
        uint32_t end;
    } ADDR_RANGE;

    // Define ranges
    const ADDR_RANGE addrRange[] = {
        {ADE9178_REG_AVGAIN, ADE9178_REG_CONFIG_LOCK},
        {ADE9178_REG_AVRMS, ADE9178_REG_CRC_RSLT},
        {ADE9178_REG_STATUS0, ADE9178_REG_VERSION2},
        {ADE9178_REG_AVRMS_1, ADE9178_REG_AUX5RMSHALF_1},
        {ADE9178_REG_AVRMS_2, ADE9178_REG_VACRMSONE_2},
    };

    uint32_t i, nextAddr = addr;

    for (i = 0; i < numRegisters; i++)
    {
        // Check if the current address falls within any range
        for (size_t j = 0; j < sizeof(addrRange) / sizeof(addrRange[0]); j++)
        {
            if (nextAddr > addrRange[j].end)
            {
                nextAddr = addrRange[j].start;
                break;
            }
        }

        INFO_MSG("Address = 0x%x    Value = 0x%x\t", nextAddr, pRegData[i]);
        nextAddr++;
    }
}
/**
 * @}
 */
