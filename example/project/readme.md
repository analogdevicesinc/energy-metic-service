# ADE9178 Evaluation CLI 

## Overview

This application provides  command line interface to communicate with the ADE9178 using the MetIC Service. It processes commands received via the CLI Service to perform operations on the ADE9178.

**Module used:**
- **Energy Board Support:** SPI UART/Driver abstractions for the evaluation board.
- **Energy Firmware Services:** Includes CLI, CRC, and NVM services utilized by the application.
- **MAX32670 Drivers:** Provides support for the MAX32670 MCU.

All modules except the MAXIM drivers are included in source form. MAXIM drivers can be obtained from [GitHub](https://github.com/analogdevicesinc/msdk/tree/v2024_10) or by following the [MAXIM Installation steps](https://analogdevicesinc.github.io/msdk//USERGUIDE/#installation).

**Supported Features:**
- Outputs data every 20 ms in units specified by the ADE9178 datasheet.
- Monitors and displays error status on emulator (Tera Term or PuTTY).
- Stores all configuration settings in Flash memory by issuing appropriate command.
- Supports waveform sample collection (PCF or ADC samples).

For communication details, GUI information, and supported CLI commands, refer to the [User Guide](../../docs/ADSW-ADE9178-MET-EXAMPLE.1.1.0_UsersGuide.pdf).


