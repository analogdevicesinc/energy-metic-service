# Porting Examples to a Custom Board

## Overview
This guide describes how to adapt example projects—originally designed for [EVAL ADE9178](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/eval-ade9178.html#eb-overview)— to a different board.

## Static Configuration
The example expects a configuration folder at [include/config](include/config) containing `app_cfg.h`for static settings.

- In [`app_cfg.h`](../include/config/app_cfg.h), update the macros to match your system requirements.

## Driver Integration

To run the example on a custom board, update the following:

1. **MCU Board Support**  
    Example projects uses MAX32670 drivers.
    - Provide implementations for your board for all the functions prefixed with `Evb` referred from example to 

2. **ADE9178 SPI Interface**  
    The ADE9178 communicates with the MCU via SPI 
    - Connect the ADE9178 SPI pins to the appropriate  SPI interface.
    - Implement SPI initilization and set priorities appropriately.

3. **ADE9178 Waveform Streaming**
    The example uses UART to capture samples from ADE9178. 
      - Connect the ADE9178 WFRM_OUT pins to the appropriate  UART pin.
      - Implement UART initilization and set priorities appropriately.
   

 4. **UART for CLI**
    The example uses UART for CLI. 
      - Implement UART initilization and set priorities appropriately.

5. **GPIO Pin Mapping**  
    The example uses several GPIOs to monitor ADE9178 interrupts (e.g., HOST_ERR, HOST_RDY, IRQ0–IRQ3, CF1, CF2, ADC_RESET, ADE_RESET).

    - **Input Pins:**  
      - HOST_ERR: P0_21  
      - HOST_RDY: P0_22  
      - IRQ0: P0_27  
      - IRQ1: P0_26  
      - IRQ2: P0_25  
      - IRQ3: P0_24  
      - CF1: P0_6  
      - CF2: P0_20  

    - **Output Pins:**  
      - ADC_RESET: P0_18  
      - ADE_RESET: P0_19  

    Update pin assignments as needed for your hardware.

## Next Steps

After making the required code and configuration changes, build and run the example using the instructions in [readme.md](readme.md).