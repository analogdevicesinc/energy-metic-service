/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/
#include "adi_evb.h"
#include "adi_metic.h"
#include "app_cfg.h"
#include "metic_service_interface.h"
#include <string.h>

static ADI_METIC_HANDLE hAde;
static METIC_INSTANCE_INFO meticIf;
static ADI_EVB_CONFIG evbConfig;
static void *hEvb;

/**
 * @brief Device Type
 * This enum defines the device types that can be used with the Metrology Service.
 */
typedef enum
{
    /** Ade9178 device type*/
    DEVICE_ADE9178 = 0x0,
    /** ADC0 device type*/
    DEVICE_ADC0,
    /** ADC1 device type*/
    DEVICE_ADC1,
    /** ADC2 device type*/
    DEVICE_ADC2,
    /** ADC3 device type*/
    DEVICE_ADC3,
    /** ALL_ADC device type*/
    DEVICE_ALL_ADC,
} DEVICE_TYPE;

int main()
{
    /* This example demonstrates the following:
     * 1. Creates the instance of metrology service thats required to call the metrology service
     * APIs.
     * 2. Resets the ADE9178 and ADC and checks for the startup status of the IC.
     * 3. If its successful, then writes to and read from an ADE9178 register. Refer to the Read and
     * write APIs to choose the different devices.
     * 4. Sends the written data to the host using UART.
     */

    uint8_t device = DEVICE_ADE9178;
    uint8_t address = 0;
    uint8_t numRegisters = 1;
    int32_t readValue = 0;
    int32_t writeValue = 2;
    int32_t boardStatus = 0;
    METIC_INSTANCE_INFO *pMeticIf = &meticIf;
    ADI_EVB_CONFIG *pEvbConfig = &evbConfig;
    pEvbConfig->gpioConfig.pfGpioCallback = MetIcIfGPIOCallback;

    boardStatus = EvbInit(&hEvb, pEvbConfig);
    if (boardStatus == 0)
    {

        // Create instance for Metrology Service, intialises the instance and populates the
        // configurations.
        MetIcIfCreateInstance(pMeticIf);
        // Reset the ADE9178 IC and ADC
        EvbResetAde();
        // Checks the startup status of the IC and waits for the IRQ0 to come up and then starts the
        // ADC.
        MetIcIfStartAdc(pMeticIf);

        // Write to the ADE9178 with the register address = 0x0 and value = 0x02.
        adi_metic_WriteRegister(hAde, device, address, &writeValue);
        // Read back the value from the ADE9178 with the register address = 0x0
        adi_metic_ReadRegister(hAde, device, address, numRegisters, &readValue);
        // Send samples to host
        EvbStartHostUartTx(hEvb, (uint8_t *)&readValue, numRegisters * sizeof(int32_t));
    }
    while (1)
    {
    }
}

int32_t MetIcIfSuspend(void *pInfo, volatile uint8_t *pSuspendState)
{
    int32_t status = 1;
    uint32_t waitCount = 0;
    if (pInfo != NULL)
    {
        status = 0;
        while ((*pSuspendState == 1) && (waitCount < APP_CFG_TIMEOUT_COUNT))
        {
            waitCount++;
        }
        *pSuspendState = 1;
        if (waitCount == APP_CFG_TIMEOUT_COUNT)
        {
            status = -1;
        }
    }

    return status;
}

int32_t MetIcIfResume(void *pInfo, volatile uint8_t *pSuspendState)
{
    int32_t status = 1;
    if (pInfo != NULL)
    {
        *pSuspendState = 0;
    }

    return status;
}

void AdeInitHal(ADI_METIC_HANDLE hInstance)
{
    hAde = hInstance;
}

void AdeHandleHostRdyErrCallback(uint32_t flag)
{
    uint8_t pinState;
    if ((flag & BOARD_CFG_HOST_RDY_PIN) != 0)
    {
        pinState = (uint8_t)MXC_GPIO_InGet(BOARD_CFG_ADECOMM_PORT, BOARD_CFG_HOST_RDY_PIN);
        adi_metic_HostRdyCallback(hAde, pinState);
    }

    if ((flag & BOARD_CFG_HOST_ERR_PIN) != 0)
    {
        pinState = (uint8_t)MXC_GPIO_InGet(BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_HOST_ERR_PIN);
        adi_metic_HostErrCallback(hAde, pinState);
    }
}
