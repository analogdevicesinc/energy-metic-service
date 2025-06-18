/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        error_display.c
 * @brief       Definitions for handling and displaying error messages
 * @{
 */
/*==========================  I N C L U D E S   ==========================*/

#include "error_display.h"
#include "adi_metic_status.h"
#include "message.h"
#include "metic_example.h"
#include "status.h"
#include <stdint.h>

/** Error status description of of ADE9178 */
char *ade9178ErrorCodes[] = {"Invalid address",
                             "Invalid Device ID",
                             "Invalid number of registers",
                             "Address read only",
                             "Insufficient BaudRate",
                             "Configuration registers locked",
                             "Invalid ADC Redirect slot Configuration",
                             "Command Failed to execute",
                             "Adc not Intialised",
                             "Issue in SPI communication between ADE9178 and Host MCU",
                             "Syncronization in progress"};

/** Error status description of of ADE */
char *adeCmdErrorCodes[] = {"Ade9178 returned error code", "No Response from ADE9178",
                            "Invalid Number of Registers", "CRC mismatch in response",
                            "Command send failed"};

/** ERROR_STATUS register description */
char *errorStatusCodes[] = {"Adc Init Error",
                            "Error0",
                            "Error1",
                            "Error2",
                            "Error3",
                            "Error4",
                            "Adc Crc Error",
                            "Error5",
                            "Dready Frequency Error",
                            "ADC0 Status0 Error",
                            "ADC0 Status1 Error",
                            "ADC0 Status2 Error",
                            "ADC1 Status0 Error",
                            "ADC1 Status1 Error",
                            "ADC1 Status2 Error",
                            "ADC2 Status0 Error",
                            "ADC2 Status1 Error",
                            "ADC2 Status2 Error",
                            "ADC3 Status0 Error",
                            "ADC3 Status1 Error",
                            "ADC3 Status2 Error",
                            "Error6",
                            "Error7"};

static void DisplayErrorStatusCodes(int32_t errorStatus);

int32_t HandleError(int32_t status)
{
    int32_t outStatus = SYS_STATUS_SUCCESS;

    switch (status)
    {
    case SYS_STATUS_SUCCESS:
    case SYS_STATUS_RUNNING:
    case SYS_STATUS_NOT_STARTED:
    case SYS_STATUS_EXAMPLE_BUFFER_FULL:
    case SYS_STATUS_MET_PROCESS:
        break;
    case SYS_STATUS_SPI_COMM_ERROR:
        WARN_MSG("SPI Init Failed")
        break;
    case SYS_STATUS_CLI_INIT_FAILED:
        WARN_MSG("Uart Init Failed")
        /* No point in sending any error messages*/
        break;
    case SYS_STATUS_IRQ0_TIME_OUT_ERROR:
        WARN_MSG("Timeout while waiting for IRQ0")
        break;
    case SYS_STATUS_EVB_INIT_FAILURE:
        WARN_MSG("Evb Init Failed")
        break;
    case SYS_STATUS_ADE_INIT_FAILURE:
        WARN_MSG("Ade Init Failed")
        break;
    case SYS_STATUS_EXAMPLE_INIT_FAILURE:
        WARN_MSG("Example Init Failed")
        break;
    default:
        WARN_MSG("Error code 0x%x", status)
        break;
    }

    return outStatus;
}

void DisplayErrorCode(int32_t address, int32_t cmdType, int32_t regData, ADI_METIC_STATUS errorCode)
{
    uint32_t offset;

    if (errorCode == ADI_METIC_STATUS_METIC_RETURNED_ERROR)
    {
        if (cmdType == 1)
        {
            ERROR_MSG("ADE9178 error code : 0x%x (%s) while reading address 0x%0x", regData,
                      ade9178ErrorCodes[regData - 1], address)
        }
        else
        {
            ERROR_MSG("ADE9178 error code : 0x%x (%s) while writing to address 0x%0x", regData,
                      ade9178ErrorCodes[regData - 1], address)
        }
    }
    else
    {
        /** The error display is for read and write register*/
        if (errorCode >= ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC &&
            errorCode <= ADI_METIC_STATUS_COMM_ERROR)
        {
            offset = errorCode - ADI_METIC_STATUS_METIC_RETURNED_ERROR;
            if (cmdType == 1)
            {
                ERROR_MSG("HOST MCU Error Code : 0x%x (%s) while reading address 0x%0x", errorCode,
                          adeCmdErrorCodes[offset], address)
            }
            else
            {
                ERROR_MSG("HOST MCU Error Code : 0x%x (%s) while writing to address 0x%0x",
                          errorCode, adeCmdErrorCodes[offset], address)
            }
        }
        else if (errorCode > ADI_METIC_STATUS_LAST_ERROR)
        {
            ERROR_MSG("Invalid error code from HOST MCU is 0x%0x", errorCode)
        }
    }
}

int32_t DisplayErrorStatusMessage(METIC_EXAMPLE *pExample)
{
    ADI_METIC_STATUS adeStatus = 0;
    int32_t statusFlag = 0;
    int32_t errorRegStatus = 0;
    int32_t errorStatusDisplay = pExample->exampleConfig.displayConfig.enableErrorStatusOutput;

    adeStatus = MetIcIfGetErrorStatus(&pExample->adeInstance, &errorRegStatus);
    if ((errorRegStatus != 0) && (adeStatus == 0))
    {
        if (errorStatusDisplay == 1)
        {
            ERROR_MSG("Error Status :  0x%x", errorRegStatus)
            DisplayErrorStatusCodes(errorRegStatus);
        }
        else
        {
            if (pExample->prevErrorStatus != errorRegStatus)
            {
                pExample->prevErrorStatus = errorRegStatus;
                ERROR_MSG("Error Status :  0x%x", errorRegStatus)
                DisplayErrorStatusCodes(errorRegStatus);
            }
        }
        errorRegStatus = 0;
        statusFlag = 1;
    }
    return statusFlag;
}

void DisplayErrorStatusCodes(int32_t errorStatus)
{
    uint32_t i;
    uint32_t numErrorStatusBits = 23;
    for (i = 0; i < numErrorStatusBits; i++)
    {
        if (((errorStatus >> i) & 0x1) == 1)
        {
            ERROR_MSG(errorStatusCodes[i])
        }
    }
}

void DisplayErrorStatusCount(void)
{
    uint32_t i;
    uint32_t numErrorStatusBits = 23;
    int32_t errorStatusFlag = 0;
    METIC_INSTANCE_INFO *pInfo = GetAdeInstance();
    for (i = 0; i < numErrorStatusBits; i++)
    {
        if (pInfo->errorStatusCount[i] > 0)
        {
            errorStatusFlag = 1;
            ERROR_MSG("%s occured is  %d times", errorStatusCodes[i], pInfo->errorStatusCount[i])
        }
    }
    if (errorStatusFlag == 0)
    {
        INFO_MSG("No errors occured")
    }
}

/**
 * @}
 */
