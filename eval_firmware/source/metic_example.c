/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_example.c
 * @brief       Definitions for intialising, setting configurations and running the example
 * @{
 */

#include "metic_example.h"
#include "ade9178.h"
#include "adi_cli.h"
#include "adi_metic.h"
#include "app_cfg.h"
#include "error_display.h"
#include "example_display.h"
#include "message.h"
#include "metic_hal.h"
#include "metic_service_interface.h"
#include "status.h"

/**
 *  Top level structure for MET application
 */
static METIC_EXAMPLE adeExample;

/**
 *  populates Example configuration structure.
 */
static int32_t PopulateAdeExampleStruct(void);
static void HostUartRxCallback(void);
static void HostUartTxCallback(void);
static void HandleWaveformCaptureAndMissedCounts(METIC_EXAMPLE *pExample);

METIC_EXAMPLE_CONFIG *GetExampleConfig(void)
{
    METIC_EXAMPLE *pExample = &adeExample;
    return &pExample->exampleConfig;
}

METIC_INSTANCE_INFO *GetAdeInstance(void)
{
    METIC_EXAMPLE *pExample = &adeExample;
    return &pExample->adeInstance;
}

int32_t InitBoard(void)
{
    int32_t status = 0;
    METIC_EXAMPLE *pExample = &adeExample;
    ADI_EVB_CONFIG *pEvbConfig = &pExample->evbConfig;

    pEvbConfig->gpioConfig.pfGpioCallback = MetIcIfGPIOCallback;
    pEvbConfig->uartConfig.pfWfsUartRxCallback = MetIcIfWfsUartCallBack;
    pEvbConfig->uartConfig.pfHostUartRxCallback = HostUartRxCallback;
    pEvbConfig->uartConfig.pfHostUartTxCallback = HostUartTxCallback;

    status = EvbInit(&pExample->hEvb, pEvbConfig);
    EvbLedFlash(0, 10, 25);
    EvbLedFlash(1, 10, 25);
    if (status != 0)
    {
        status = SYS_STATUS_EVB_INIT_FAILURE;
    }
    return status;
}

void HostUartRxCallback(void)
{
    adi_cli_RxCallback();
}

void HostUartTxCallback(void)
{
    adi_cli_TxCallback();
}

int32_t InitServices(void)
{
    METIC_EXAMPLE *pExample = &adeExample;
    int32_t status;
    ADI_METIC_STATUS adeStatus;
    status = (CliCreateInstance(&pExample->cliIf) != 0) ? SYS_STATUS_CLI_INIT_FAILED
                                                        : SYS_STATUS_SUCCESS;
    if (status == SYS_STATUS_SUCCESS)
    {
        status = NvmCreateInstance(&pExample->nvmIf);
        if (status == SYS_STATUS_SUCCESS)
        {
            adeStatus = MetIcIfCreateInstance(&pExample->adeInstance);
            /* This is only to check CF intervals if required*/
            PcntInit(&pExample->pcntInfo, APP_CFG_MAX_NUM_IRQ_TIME, &pExample->pcntBuffer[0]);
            if (adeStatus != ADI_METIC_STATUS_SUCCESS)
            {
                status = SYS_STATUS_ADE_INIT_FAILURE;
            }
        }
    }

    return status;
}

int32_t CollectMetOutputs(void)
{
    int32_t status = SYS_STATUS_RUNNING;
    METIC_EXAMPLE *pExample = &adeExample;

    // Read metrology parameters if cycles are not completed
    if (pExample->processedCycles < pExample->exampleConfig.cyclesToRun)
    {
        int32_t outputStatus =
            MetIcIfReadMetrologyParameters(&pExample->adeInstance, &pExample->output);

        // Handle output status
        switch (outputStatus)
        {
        case 0: // Success
            if (pExample->adeInstance.enableRegisterRead)
            {
                DisplayOutput(&pExample->exampleConfig.displayConfig,
                              pExample->adeInstance.irqStatus.irq0Count, &pExample->output);
            }
            pExample->processedCycles++;
            break;

        case SYS_STATUS_IRQ0_TIME_OUT_ERROR:
            ERROR_MSG("Timeout error in IRQ0");
            break;

        case SYS_STATUS_SPI_COMM_ERROR:
            ERROR_MSG("Errors detected in reading output registers");
            pExample->processedCycles = pExample->exampleConfig.cyclesToRun;
            break;

        default:
            break;
        }
    }
    else
    {
        // Handle waveform capture and missed counts
        HandleWaveformCaptureAndMissedCounts(pExample);
        status = SYS_STATUS_NOT_STARTED;
        pExample->state = METIC_EXAMPLE_STATE_WAITING_FOR_START_CMD;
    }

    return status;
}

void HandleWaveformCaptureAndMissedCounts(METIC_EXAMPLE *pExample)
{
    // Handle waveform capture
    if (pExample->adeInstance.enableWfsCapture)
    {
        ADI_METIC_STATUS adeStatus =
            MetIcIfCollectSamples(&pExample->adeInstance, pExample->exampleConfig.wfsConfig);
        if (adeStatus != 0 && adeStatus != ADI_METIC_STATUS_WFS_TRANSACTION_COMPLETED)
        {
            ERROR_MSG("Failed to stream samples. Error code from Metrology Service is %d",
                      adeStatus);
        }
    }

    // Handle missed counts
    if (pExample->adeInstance.enableRegisterRead)
    {
        int32_t missedCount = pExample->adeInstance.irqStatus.irq0Count - pExample->processedCycles;
        if (missedCount > 0)
        {
            WARN_MSG("Missed count of sending metrology outputs through UART is %d", missedCount);
            INFO_MSG(
                "Try enabling single output parameters to reduce missed count of output display. "
                "Refer to setdisplay command for list of output parameters to select.");
        }
    }
}
int32_t InitExampleStates(void)
{
    int32_t status = 0;
    ADI_METIC_STATUS adeStatus = ADI_METIC_STATUS_SUCCESS;
    METIC_EXAMPLE *pExample = &adeExample;
    METIC_EXAMPLE_CONFIG *pConfig = &pExample->exampleConfig;
    int32_t mask0 = ADE9178_BITM_MASK0_RMSONERDY;
    InitDisplayConfig(&pExample->exampleConfig.displayConfig);
#if BOARD_CFG_RESET_TYPE == 0
    status = HandleResetCmd(RESET_DEVICE_ADE9178);
#else
    status = HandleResetCmd(RESET_DEVICE_ALL);
#endif
    if (status == 0)
    {
        status = LoadNvmReg(pExample);
        if ((status == 0) || (status == 1))
        {
            status = 0;
            adeStatus =
                adi_metic_WriteRegister(pExample->adeInstance.hAde, 0, ADE9178_REG_MASK0, &mask0);
            if (adeStatus != ADI_METIC_STATUS_SUCCESS)
            {
                DisplayErrorCode(ADE9178_REG_MASK0, 0, mask0, adeStatus);
                status = -1;
            }
        }
    }

    if (status == 0)
    {
        /* Do some state initialization*/
        pConfig->cyclesToRun = 0;
        pExample->state = METIC_EXAMPLE_STATE_WAITING_FOR_START_CMD;
        pExample->processedCycles = 0;
    }
    if (status != SYS_STATUS_SUCCESS)
    {
        status = SYS_STATUS_EXAMPLE_INIT_FAILURE;
    }
    return status;
}

int32_t HandleRunState(void)
{
    int32_t status = SYS_STATUS_RUNNING;
    METIC_EXAMPLE *pExample = &adeExample;
    switch (pExample->state)
    {
    case METIC_EXAMPLE_STATE_WAITING_FOR_START_CMD:
        /* We will be in this state till we get a start command*/
        status = SYS_STATUS_NOT_STARTED;
        break;
    case METIC_EXAMPLE_STATE_RUNNING:
        status = CollectMetOutputs();
        break;
    }

    DisplayErrorStatusMessage(pExample);
    return status;
}

int32_t HandleResetCmd(int32_t device)
{
    int32_t status = 0;
    int32_t mask0 = ADE9178_BITM_MASK0_RMSONERDY;
    ADI_METIC_STATUS adeStatus = ADI_METIC_STATUS_SUCCESS;
    METIC_EXAMPLE *pExample = &adeExample;

    if (device == RESET_DEVICE_ADE9178)
    {
        EvbResetAde();
    }
    else if (device == RESET_DEVICE_ADC)
    {
        EvbResetAdcs();
    }
    else
    {
        EvbResetAll();
    }
    adeStatus = MetIcIfStartAdc(&pExample->adeInstance);
    if (adeStatus != ADI_METIC_STATUS_SUCCESS)
    {
        if (adeStatus == ADI_METIC_STATUS_METIC_RETURNED_ERROR)
        {
            ERROR_MSG("Failed to run ADC. Error Code from ADE9178 is %d", adeStatus)
        }
        else
        {
            ERROR_MSG("Failed to run ADC. Error Code from Metrology Service is %d", adeStatus)
        }
    }
    PopulateAdeExampleStruct();
    pExample->prevErrorStatus = 0;

    if (adeStatus == ADI_METIC_STATUS_SUCCESS)
    {
        adeStatus =
            adi_metic_WriteRegister(pExample->adeInstance.hAde, 0, ADE9178_REG_MASK0, &mask0);
        if (adeStatus != ADI_METIC_STATUS_SUCCESS)
        {
            DisplayErrorCode(ADE9178_REG_MASK0, 0, mask0, adeStatus);
        }
    }
    if (adeStatus != ADI_METIC_STATUS_SUCCESS)
    {
        status = -1;
    }
    return status;
}

int32_t HandleStartCmd(void)
{
    int32_t status = 0;
    METIC_EXAMPLE *pExample = &adeExample;
    ADI_METIC_STATUS adeStatus = ADI_METIC_STATUS_SUCCESS;
    pExample->processedCycles = 0;
    adeStatus = MetIcIfResetIrqStatus(&pExample->adeInstance);
    pExample->adeInstance.irqStatus.lastIrqTime = EvbGetTime();
    if (adeStatus == ADI_METIC_STATUS_SUCCESS)
    {
        pExample->state = METIC_EXAMPLE_STATE_RUNNING;
    }
    else
    {
        WARN_MSG("Unable to clear STATUS0 register")
    }

    return status;
}

int32_t HandleSaveRegCmd(void)
{
    int32_t status = 0;
    METIC_EXAMPLE *pExample = &adeExample;
    ADE_CONFIG_REG *pConfig = &pExample->adeConfig;
    status = SaveNvmReg(pExample);
    if (status == 0)
    {
        DisplayNvmReg(pConfig);
    }
    return status;
}

int32_t HandleLoadRegCmd(void)
{
    int32_t status = 0;
    METIC_EXAMPLE *pExample = &adeExample;
    ADE_CONFIG_REG *pConfig = &pExample->adeConfig;
    status = LoadNvmReg(pExample);
    if (status == 0)
    {
        DisplayNvmReg(pConfig);
    }
    return status;
}

void HandleSetConfigCmd(uint16_t configChoice, float value)
{
    METIC_EXAMPLE_CONFIG *pConfig = &adeExample.exampleConfig;
    switch (configChoice)
    {
    case METIC_EXAMPLE_CONFIG_VOLTAGE_SCALE:
        pConfig->displayConfig.voltageScale = value;
        break;
    case METIC_EXAMPLE_CONFIG_CURRENT_SCALE:
        pConfig->displayConfig.currentScale = value;
        break;
    case METIC_EXAMPLE_CONFIG_AUX_SCALE:
        pConfig->displayConfig.auxScale = value;
        break;
    }
    DisplayConfigValue(configChoice);
}

void HandleDisplayPulseTimeCmd(uint32_t pulseId)
{
    static uint32_t pulseTime[APP_CFG_MAX_NUM_IRQ_TIME] = {0};
    uint32_t numPulses = 0;
    uint32_t i;
    uint32_t eventGpioPin;
    METIC_EXAMPLE *pExample = &adeExample;
    static uint32_t gpioPins[2] = {BOARD_CFG_CF1_PIN, BOARD_CFG_CF2_PIN};
    for (i = 0; i < pExample->adeInstance.cfIndex; i++)
    {
        PcntAddToBuffer(&pExample->pcntInfo, pExample->adeInstance.cfFlag[i],
                        pExample->adeInstance.cfTime[i]);
    }
    numPulses = PcntCopyBuffer(&pExample->pcntInfo, &pulseTime[0]);
    if (pulseId >= 2)
    {
        eventGpioPin = gpioPins[0];
    }
    else
    {
        eventGpioPin = gpioPins[pulseId];
    }
    INFO_MSG("")
    if (numPulses > 0)
    {
        for (i = 0; i < numPulses; i = i + 2)
        {

            if (pulseTime[i] & eventGpioPin)
            {
                INFO_MSG_RAW("%ld,", pulseTime[i + 1])
            }
            adi_cli_FlushMessages();
        }
        INFO_MSG("")
    }
    else
    {
        INFO_MSG("No Events occurred")
    }
}

int32_t PopulateAdeExampleStruct(void)
{
    int32_t status = 0;
    uint32_t numRegisters;
    METIC_EXAMPLE *pExample = &adeExample;
    numRegisters = sizeof(configReg) / sizeof(configReg[0]);
    // Copies the ade configuration to structure with default values.
    memcpy(&pExample->adeConfig, &configReg[0], numRegisters * sizeof(uint32_t) * 2);

    return status;
}

/**
 * @}
 */
