/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/*============= I N C L U D E S =============*/
#include "cli_commands.h"
#include "ade9178.h"
#include "ade9178_enums.h"
#include "error_display.h"
#include "example_display.h"
#include "example_version.h"
#include "message.h"
#include "metic_service_interface.h"
#include "status.h"
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/** Macro for reading register all ADCs */
#define READ_ALL_ADC_DEVICE 5
/** Macro for reading register of ADE9178 */
#define READ_ADE9178 0
/** Buffer to store the command parameter */
static char commandParam[APP_CFG_CLI_MAX_PARAM_LENGTH];

#define MAX_ADE9178_REG_LENGTH 256

/** The order should be as  DISPLAY_CONFIG structure */
static char *displayChoices[] = {"all",         "rms",          "rmsone", "rmshalf",
                                 "eventrmsone", "eventrmshalf", "power",  "energy",
                                 "period",      "angle",        "status", "errorstatus"};

static char *deviceChoices[] = {"ADE9178", "ADC0", "ADC1", "ADC2", "ADC3", "ALL_ADC"};

static char *displayDescription[] = {"all parameters",
                                     "all filtered rms output parameters",
                                     "all rmsone output parameters",
                                     "all rmshalf output parameters",
                                     "all channel dip and swell rmsone output parameters",
                                     "all channel dip and swell rmshalf output parameters",
                                     "all power output parameters",
                                     "all energy output parameters",
                                     "all period output parameters",
                                     "all angle output parameters",
                                     "all status output parameters",
                                     "Enable or disable display of repeated error status messages"

};

#if BOARD_CFG_RESET_TYPE == 1
static int32_t ResetEvb(Args *pArgs);
#else
static int32_t ResetEvk(Args *pArgs);
#endif
static void GetRegisterValue(uint8_t device, uint16_t address, uint16_t numReg);

int32_t CmdStart(Args *pArgs)
{
    int32_t status = 0;
    METIC_EXAMPLE_CONFIG *pConfig = GetExampleConfig();
    if (pArgs->c == 1)
    {
        METIC_INSTANCE_INFO *pInfo = GetAdeInstance();
        pConfig->cyclesToRun = (uint32_t)pArgs->v[0].d;
        pInfo->enableRegisterRead = 1;
        pInfo->enableWfsCapture = 0;

        status = HandleStartCmd();

        if (status != 0)
        {
            WARN_MSG("Unable to start the program")
        }
    }
    else
    {
        WARN_MSG("Wrong arguments. Use help start")
    }

    return status;
}
int32_t CmdReset(Args *pArgs)
{
#if BOARD_CFG_RESET_TYPE == 0
    ResetEvk(pArgs);
#else
    ResetEvb(pArgs);
#endif
    return 0;
}

int32_t CmdVersion(Args *pArgs)
{
    ADI_METIC_STATUS adeStatus;
    int32_t version;
    int32_t version2 = 0;
    METIC_INSTANCE_INFO *pInfo;
    if (pArgs->c == 0)
    {
        pInfo = GetAdeInstance();
        adeStatus = adi_metic_ReadRegister(pInfo->hAde, 0, ADE9178_REG_VERSION, 1, &version);
        if (adeStatus == ADI_METIC_STATUS_SUCCESS)
        {
            adeStatus = adi_metic_ReadRegister(pInfo->hAde, 0, ADE9178_REG_VERSION2, 1, &version2);
        }
        if (adeStatus == ADI_METIC_STATUS_SUCCESS)
        {
            uint32_t major;
            uint32_t minor;
            uint32_t patch;
            major = (version >> 24) & 0xFF;
            minor = (version >> 16) & 0xFF;
            patch = (version >> 8) & 0xFF;
            INFO_MSG("ADE9178 Firmware Version:%d.%d.%d", major, minor, patch)
            INFO_MSG("ADE9178 Firmware Chip ID: 0x%x", version2)
            INFO_MSG("ADE9178 Example Build ID: 0x%x", ADE9178_EXAMPLE_BUILD_HASH)
        }
        else
        {
            INFO_MSG("Not able to read version from ADE")
        }
    }
    else
    {
        WARN_MSG("Invalid number of arguments")
    }

    return 0;
}

int32_t CmdCaptureWfrm(Args *pArgs)
{
    int32_t wfsData;
    int32_t wfsEnable = 1;
    int32_t channelEnable;
    int32_t wfrmSrcType;
    int32_t cycleDelay;
    int32_t baudrate = 5;
    ADI_METIC_STATUS adeStatus = 0;
    METIC_EXAMPLE_CONFIG *pConfig = GetExampleConfig();
    METIC_INSTANCE_INFO *pInfo = GetAdeInstance();
    if (pArgs->c == 2)
    {
        wfrmSrcType = pArgs->v[0].d;
        cycleDelay = pArgs->v[1].d;
        if (wfrmSrcType == ADE9178_ADC_SAMPLES || wfrmSrcType == ADE9178_PCF_OUTPUT)
        {
            if (cycleDelay >= 0)
            {
#if ENABLE_ALL_CHANNELS == 1
                channelEnable = 0xFFF;
                wfsData = wfsEnable | (baudrate << ADE9178_BITP_WFS_CONFIG_BAUD_RATE) |
                          (channelEnable << ADE9178_BITP_WFS_CONFIG_AV_WFS_EN) |
                          (wfrmSrcType << ADE9178_BITP_WFS_CONFIG_WF_SRC);
#else
                channelEnable = 0x3F;
                wfsData = wfsEnable | (baudrate << ADE9178_BITP_WFS_CONFIG_BAUD_RATE) |
                          (channelEnable << ADE9178_BITP_WFS_CONFIG_AV_WFS_EN) |
                          (wfrmSrcType << ADE9178_BITP_WFS_CONFIG_WF_SRC);
#endif
                adeStatus =
                    adi_metic_WriteRegister(pInfo->hAde, 0, ADE9178_REG_WFS_CONFIG, &wfsData);
                if (adeStatus == ADI_METIC_STATUS_SUCCESS)
                {
                    INFO_MSG("Wfs Configuration is successful")
                    // Starts Waveform Rx Capture after completing configured cycle.
                    pConfig->cyclesToRun = (uint32_t)cycleDelay;
                    pInfo->enableRegisterRead = 0;
                    pInfo->enableWfsCapture = 1;
                    pConfig->wfsConfig = wfsData;
                    HandleStartCmd();
                }
                else
                {
                    DisplayErrorCode(ADE9178_REG_WFS_CONFIG, 0, wfsData, adeStatus);
                }
            }
            else
            {
                WARN_MSG("Invalid number of cycles")
            }
        }

        else
        {
            WARN_MSG("Invalid waveform source type")
        }
    }
    else
    {
        WARN_MSG("Wrong number of arguments. Use help capturewfrm")
    }
    return 0;
}

int32_t CmdDisplayWfrm(Args *pArgs)
{
    if (pArgs->c == 0)
    {

        DisplayWaveformOutput();
    }
    else
    {
        WARN_MSG("Wrong arguments. Use help displaywfrm")
    }
    return 0;
}

int32_t CmdDisplayErrorStatusCount(Args *pArgs)
{
    if (pArgs->c == 0)
    {
        METIC_INSTANCE_INFO *pInfo = GetAdeInstance();
        DisplayErrorStatusCount();
        memset(&pInfo->errorStatusCount[0], 0, 23 * sizeof(uint32_t));
    }
    else
    {
        WARN_MSG("Wrong arguments. Use help geterrorcount")
    }
    return 0;
}

int32_t CmdLoadReg(Args *pArgs)
{
    int32_t status = 0;
    if (pArgs->c == 0)
    {
        status = HandleLoadRegCmd();
        if (status == 0)
        {
            INFO_MSG("Registers loaded successfully.")
        }
        else if (status == -1)
        {
            WARN_MSG("Failed to load registers.")
        }
    }
    else
    {
        WARN_MSG("Invalid number of arguments")
    }
    return 0;
}

int32_t CmdSaveReg(Args *pArgs)
{
    int32_t status = 0;
    if (pArgs->c == 0)
    {
        status = HandleSaveRegCmd();
        if (status == 0)
        {
            INFO_MSG("Registers saved to flash successfully.")
        }
        else
        {
            WARN_MSG("Failed to save registers")
        }
    }
    else
    {
        WARN_MSG("Invalid number of arguments")
    }
    return 0;
}

int32_t CmdSetDisplay(Args *pArgs)
{
    METIC_EXAMPLE_CONFIG *pConfig = GetExampleConfig();
    /* Assumes structure contains only boolean flags*/
    uint8_t *pDisplayConfig = (uint8_t *)&pConfig->displayConfig;
    char *pParam = &commandParam[0];
    int32_t numChoices = sizeof(displayChoices) / sizeof(displayChoices[0]);
    int32_t choice;
    if (pArgs->c > 0)
    {
        choice = GetChoice(displayChoices, pArgs->v[0].pS, numChoices, pParam);

        if (choice >= 0)
        {
            if (pArgs->c > 1)
            {
                if (strcmp(pArgs->v[1].pS, "on") == 0)
                {
                    pDisplayConfig[choice] = true;
                    INFO_MSG("display %s set to on", pArgs->v[0].pS)
                }
                else
                {
                    INFO_MSG("display %s set to off", pArgs->v[0].pS)
                    pDisplayConfig[choice] = false;
                }

                if (choice == 0)
                {
                    // Set all configs
                    for (choice = 0; choice < numChoices; choice++)
                    {
                        pDisplayConfig[choice] = pDisplayConfig[0];
                    }
                }
            }
            else
            {
                WARN_MSG("Wrong number of arguments. Use help setdisplay for usage")
            }
        }
        else
        {
            WARN_MSG("Unsupported  display option %s", pArgs->v[0].pS)
        }
    }
    else
    {
        WARN_MSG("Wrong number of arguments. Use help setdisplay for usage")
    }

    return 0;
}
int32_t CmdSetConfig(Args *pArgs)
{
    uint16_t configChoice;
    float configValue;
    char *pEndPtr;
    if (pArgs->c == 2)
    {
        sscanf(pArgs->v[0].pS, "%" PRIx16, &configChoice);
        if (configChoice > 0 && configChoice <= 3)
        {
            configValue = strtof(pArgs->v[1].pS, &pEndPtr);
            if (*pEndPtr != '\0')
            {
                WARN_MSG("Invalid input: provide a valid number")
            }
            else
            {
                HandleSetConfigCmd(configChoice, configValue);
            }
        }
        else
        {
            WARN_MSG("Invalid config choice")
        }
    }
    else
    {
        WARN_MSG("Wrong number of arguments. Use help setconfig for usage")
    }

    return 0;
}

int32_t CmdGetConfig(Args *pArgs)
{
    uint16_t configChoice;
    if (pArgs->c == 1)
    {
        sscanf(pArgs->v[0].pS, "%" PRIx16, &configChoice);
        if (configChoice > 0 && configChoice <= 3)
        {
            DisplayConfigValue(configChoice);
        }
        else
        {
            WARN_MSG("Invalid config choice")
        }
    }
    else
    {
        WARN_MSG("Wrong number of arguments. Use help setconfig for usage")
    }

    return 0;
}

int32_t CmdSetRegData(Args *pArgs)
{
    int32_t status = 0;
    ADI_METIC_STATUS adeStatus = 0;
    uint16_t regAddress;
    int32_t regWriteData = 0;
    uint8_t device = 0;
    uint32_t i;
    int32_t numChoices = sizeof(deviceChoices) / sizeof(deviceChoices[0]);
    int32_t choice = 0;
    int32_t validnumStatus = 0;
    METIC_INSTANCE_INFO *pInfo;
    char *pParam = &commandParam[0];
    uint32_t tempValue;

    if (pArgs->c == 3)
    {
        choice = GetChoice(deviceChoices, pArgs->v[0].pS, numChoices, pParam);
        if (choice >= 0 && choice <= numChoices)
        {
            device = (uint8_t)choice;
        }
        else
        {
            WARN_MSG("Wrong Device. Use help")
            status = 1;
        }
        if (status == 0)
        {
            if (strlen(pArgs->v[1].pS) < 4)
            {
                pInfo = GetAdeInstance();
                sscanf(pArgs->v[1].pS, "%03hx" PRIx16, &regAddress);
                if (pArgs->c == 3)
                {
                    sscanf(pArgs->v[2].pS, "%08" SCNx32, &tempValue);
                    regWriteData = (int32_t)tempValue;
                    // Validate the input contains only valid hexadecimal characters
                    for (i = 0; i < strlen(pArgs->v[2].pS); i++)
                    {
                        if (!isxdigit((unsigned char)pArgs->v[2].pS[i]))
                        {

                            validnumStatus = 1;
                            break;
                        }
                    }
                    if ((strlen(pArgs->v[2].pS) > 8) || (validnumStatus == 1))
                    {
                        WARN_MSG("Invalid input: Please provide a valid 8-digit hexadecimal value")
                    }
                    else
                    {
                        adeStatus =
                            adi_metic_WriteRegister(pInfo->hAde, device, regAddress, &regWriteData);
                        if (adeStatus == ADI_METIC_STATUS_SUCCESS)
                        {
                            INFO_MSG("Value = %x written successfully", regWriteData)
                        }
                        else
                        {
                            DisplayErrorCode(regAddress, 0, regWriteData, adeStatus);
                        }
                    }
                }
                else
                {
                    WARN_MSG("Wrong arguments. Use help ")
                }
            }
            else
            {
                WARN_MSG("Please Enter 3digit Hex Address")
            }
        }
    }
    else
    {
        WARN_MSG("Wrong arguments. Use help ")
    }
    return 0;
}

int32_t CmdGetRegData(Args *pArgs)
{
    int32_t status = 0;
    uint16_t address = 0;
    uint32_t regAddress = 0;
    uint16_t numReg = 0;
    uint16_t numRegToCopy = 0;
    uint8_t device = 0;
    int32_t choice = 0;
    int32_t numChoices = sizeof(deviceChoices) / sizeof(deviceChoices[0]);
    char *pParam = &commandParam[0];
    if (pArgs->c == 3)
    {
        choice = GetChoice(deviceChoices, pArgs->v[0].pS, numChoices, pParam);
        if (choice >= 0 && choice <= numChoices)
        {
            device = (uint8_t)choice;
        }
        else
        {
            WARN_MSG("Wrong Device. Use help")
            status = 1;
        }
        if (status == 0)
        {
            if (strlen(pArgs->v[1].pS) < 4)
            {

                sscanf(pArgs->v[1].pS, "%03" PRIx32, &regAddress);
                sscanf(pArgs->v[2].pS, "%03hu" PRIx32, &numReg);
                if (device == READ_ADE9178)
                {
                    numRegToCopy = numReg;
                }
                else if (device == READ_ALL_ADC_DEVICE)
                {
                    numRegToCopy = 2;
                }
                else
                {
                    numRegToCopy = 1;
                }
                address = (uint16_t)regAddress;
                GetRegisterValue(device, address, numRegToCopy);
            }
            else
            {
                WARN_MSG("Please Enter 3 digit Hex Address")
            }
        }
    }
    else
    {
        WARN_MSG("Wrong arguments. Use help ")
    }
    return 0;
}

void GetRegisterValue(uint8_t device, uint16_t address, uint16_t numReg)
{
    ADI_METIC_STATUS adeStatus;
    uint16_t idx = 0;
    METIC_INSTANCE_INFO *pInfo;
    int32_t regValue;
    pInfo = GetAdeInstance();
    adeStatus = adi_metic_ReadRegister(pInfo->hAde, device, address, numReg, &pInfo->regBuffer[0]);

    if (adeStatus == ADI_METIC_STATUS_SUCCESS)
    {
        if (device == READ_ADE9178)
        {
            DisplayBurstRegisters(address, numReg, &pInfo->regBuffer[0]);
        }
        else
        {
            for (idx = 0; idx < numReg; idx++)
            {
                if (device == READ_ALL_ADC_DEVICE)
                {
                    INFO_MSG("Address = 0x%x    Value = 0x%x\t", address,
                             (pInfo->regBuffer[idx] >> 8) & 0xFF)
                    INFO_MSG("Address = 0x%x    Value = 0x%x\t", address,
                             (pInfo->regBuffer[idx] >> 24) & 0xFF)
                }
                else
                {
                    regValue = (pInfo->regBuffer[idx] >> 8) & 0xFF;
                    INFO_MSG("Address = 0x%x    Value = 0x%x\t", address, regValue)
                }
            }
        }
    }
    else
    {
        DisplayErrorCode(address, 1, pInfo->regBuffer[0], adeStatus);
    }
}
int32_t CmdDisplayPulseTime(Args *pArgs)
{
    int32_t srcId;
    if (pArgs->c == 1)
    {
        srcId = pArgs->v[0].d;
        if (srcId >= 0 && srcId <= 1)
        {
            HandleDisplayPulseTimeCmd((uint32_t)srcId);
        }
        else
        {
            WARN_MSG("Invalid choice")
        }
    }
    else
    {
        WARN_MSG("Invalid number of arguments")
    }

    return 0;
}

#ifdef ENABLE_X86_BUILD
int32_t CliClose(Args *pArgs)
{
    METIC_INSTANCE_INFO *pInfo;
    int32_t status = 0;

    if (pArgs->c > 0)
    {
        WARN_MSG("Incorrect usage")
    }
    else
    {
        pInfo = GetAdeInstance();
        MetIcIfClose(pInfo);
    }

    return 0;
}
#endif

void DescDisplay(void)
{
    int32_t i;
    int32_t numChoices = sizeof(displayChoices) / sizeof(displayChoices[0]);
    INFO_MSG("\t  Enable or disable display of parameters"
             "\r\n\t  Supported display options are\n")
    for (i = 0; i < numChoices; i++)
    {
        INFO_MSG("\t  %-26s  - %s", displayChoices[i], displayDescription[i])
    }
}

#if BOARD_CFG_RESET_TYPE == 1
int32_t ResetEvb(Args *pArgs)
{
    int32_t status = 0;
    if (pArgs->c == 1)
    {
        if (!(strcmp(pArgs->v[0].pS, "ade9178")))
        {
            status = HandleResetCmd(RESET_DEVICE_ADE9178);
            if (status == 0)
            {
                INFO_MSG("Reset ADE9178 successful")
            }
        }
        else if (!strcmp(pArgs->v[0].pS, "adc"))
        {
            status = HandleResetCmd(RESET_DEVICE_ADC);
            if (status == 0)
            {
                INFO_MSG("Reset ADC successful")
            }
        }
        else if (!(strcmp(pArgs->v[0].pS, "all")))
        {

            status = HandleResetCmd(RESET_DEVICE_ALL);
            if (status == 0)
            {
                INFO_MSG("Reset all devices successful")
            }
        }
        else
        {
            WARN_MSG("Wrong arguments")
        }
    }
    else
    {
        WARN_MSG("Invalid number of arguments")
    }
    return status;
}
#else
int32_t ResetEvk(Args *pArgs)
{
    int32_t status = 0;
    if (pArgs->c == 0)
    {
        status = HandleResetCmd(RESET_DEVICE_ADE9178);
        if (status == 0)
        {
            INFO_MSG("Reset all devices successful")
        }
    }
    else
    {
        WARN_MSG("Invalid number of arguments")
    }
    return status;
}
#endif