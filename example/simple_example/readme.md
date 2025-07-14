# ADE9178 Simple Example

## Overview

This example demonstrates how to communicate with the ADE9178 using the Metrology Service APIs. 

The application relies on the following external modules:

- **Energy Board Support**: Provides SPI, UART, and driver abstractions for the evaluation board.
- **MAX3267x Drivers Drivers**: Drivers for the MAX32670 MCU.

All modules except the MAX3267x drivers are included in source form. To obtain the MAX32670 drivers, visit the [GitHub repository](https://github.com/analogdevicesinc/msdk/tree/v2024_10) or follow the [installation guide](https://analogdevicesinc.github.io/msdk//USERGUIDE/#installation).

## Required BSP Functions

The following Board Support Package (BSP) functions and function pointers must be implemented for this example to build and run correctly:

- `EvbInit`
- `EvbResetAde`
- `EvbStartAdeSpiTx`
- `EvbStartAdeSpiRx`
- `EvbEnableAllGPIOIrq`
- `EvbGetPinState`
- `EvbStartHostUartTx`
- `pfGpioCallback`

## Building and Running the Example

For detailed build and run instructions using CMake and Visual Studio Code, refer to the [top level readme](../readme.md).


