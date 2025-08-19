/******************************************************************************
Copyright (c) 2024 - 2025 Analog Devices Inc.
******************************************************************************/

/* "Mainpage" is the main page for the DoxyGen output.
   Use it to present overview of the chip, drivers, release notes, etc.
   Use the "@section" directive to create section headers.
   Use the "@htmlinclude" directive to import html documentation.
 */

/** @mainpage notitle

    @section Intro MetIC Service Description

     Metrology Service contains set of APIs to communicate with ADI metrology ICs and to collect metrology parameters. The APIs assume  4-wire SPI is used
     to connect to the IC. This release only supports ADE9178 metrology IC.

     The package comes with source codes for the service and examples codes to show how these APIs can be used. The example is written for EVAL-ADE9178 board.

     Note that only block marked as metrology service is part of the service.  All  other blocks are expected to be written as part of the application. Example codes are given
     to show how APIs can be called to configure and get data from metrology ICs. A CLI interface over UART is provided as user interface for the example. This can be used to configure the metrology IC and to view the metrology parameters
     collected from the IC. All calibration coefficients are stored in the internal flash of MAX32670 of EVAL-ADE9178. This functionality is abstracted into a Non Volatile Memory service (NVM) in
     the example application.

     As shown in the figure , the service expects the application to initialise and configures the required peripherals appropriately.
     Following peripherals for correct functionality.
     - SPI is connected to ADE9178. \n
          - SPI mode is 3. [clock polarity = 1, clock phase = 1] \n
          - speed > 6MHz
     - UART (To stream waveform samples, if required)
     - 2 GPIOS  for ADE9178 communication protocol, 4 GPIOs for interrupt handling, 2 GPIOs for monitoring CF pulses. \n
          - All GPIOs should be configured as inputs \n
          - HOST_RDY and HOST_ERR GPIOs interrupt mode is on both edges \n
          - IRQ0, IRQ1, IRQ2, IRQ3, CF1, CF2 interrupt mode is on falling edge. \n
     - 1 GPIOs to reset ADCs and ADE9178.

     Following callback functions should be called from appropriate ISRs\n
          - #adi_metic_HostErrCallback  -  On both edges of HOST_ERR pin \n
          - #adi_metic_HostRdyCallback  -  On both edges of HOST_RDY pin  \n
          - #adi_metic_WfsUartRxCallback -  When UART RX is completed. This is needed only to collect waveform samples from ADE9178
          - #adi_metic_Irq0Callback     -  On falling edge of IRQ0 pin


     ADE9178 also expects that ADCs are connected to ADE9178 SPI input in daisy chain mode. Refer to ADE9178 datasheet for more
     details.

     Before calling any metrology service APIs , please make sure the required peripherals are initialized and callbacks are set using appropriate MCU driver codes.
     Example codes for EVAL-ADE9178 are given along with the package.

     @subsection Init Initialization and Configuration

     The metrology service should be instantiated and configured properly before using it for collecting metrology parameters

     - #adi_metic_Create \n
     This API creates the instance of Metrology Service. One instance can handle only one metrology IC. \n

     - #adi_metic_Init \n
            Use this API to initialize and configure the service
          - Fill the #ADI_METIC_CONFIG appropriately and pass to this API. \n
               - #ADI_METIC_CONFIG.pfCmdTransfer -  A function to transmit SPI data \n
               - #ADI_METIC_CONFIG.pfResponseReceive -  A function to receive SPI data \n
               - #ADI_METIC_CONFIG.pfSuspend - A function which waits/pends till  pfResume is called.
               - #ADI_METIC_CONFIG.pfResume -  A function which stops the waiting inside pfSuspend.
               - #ADI_METIC_CONFIG.pfAddCmdCrc - A function to compute the CRC and add it to the command packet.
               - #ADI_METIC_CONFIG.pfVerifyRespCrc -  A function to verify the CRC of the response packet.
          - If waveform sample collection is needed from IC then fill following configurations also \n
               - #ADI_METIC_CONFIG.pfWfrmReceive - A function to receive UART data \n
               - #ADI_METIC_CONFIG.pfSetBaudRate - A function to set the baudrate \n
     Note that the API #adi_metic_SetConfig can be used if any reconfigurations needed to be done later. \n
     - #adi_metic_CheckStartupStatus \n
     After initialization of the service, the metrology IC can be reset by toggling the reset pin.
     This API can be used to check the startup status. Note that it may take ~16msec to power the metrology IC.
     It is recommended to call this in a loop (with an appropiate timeout value ) till the API returns SUCCESS \n

     - #adi_metic_StartAdc \n
     After metrology is started successfully , the metrology parameter calculation can be started using this API.  \n

     Following sample code shows initialization and configuration sequence. Refer to #MetIcIfCreateInstance for implementation.

     @code
     #include <adi_metic.h>

      // Initialise the peripherals SPI, GPIO, UART, Timers
      // Peripheral initialisation for UART and Timers are optional. This can be enabled or diabled through board_cfg.h file
      adi_metic_Create()

      // Fill configuration structure #ADI_METIC_CONFIG before initialisation
      adi_metic_Init()

      // Reset ADCs and ADE9178 at this point and wait for ADE9178 power up as shown below.
      // The API may return IC integrity error if there is any issue with IC or timeout waiting for IRQ0.
      while(1)
      {
           adi_metic_CheckStartupStatus()
           if(success)
           {
                break
           }
      }

      starts ADE9178 and ADCs with following API
      adi_metic_startAdc()

     @endcode


     @subsection Collecting Metrology Parameters

     After metrology IC is started, metrology parameters can be read by using API #adi_metic_ReadRegister.
     This API supports reading multiple registers at a time. It is recommended to use this feature if
     the number of registers required is large. The application can read all output registers with single
     command and extract the required registers using #adi_metic_ExtractRegisters API.

    ADE9178 output is in fixed point units and has to be converted to real units by the application. Refer to "THEORY OF OPERATION"
    subsection ADE9178 data sheet for more details. Metrology Service provides helper functions to convert following outputs
     - xRMS, xRMSONE, xRMSHALF, xDIPONE, xSWELLONE, xDIPHALF, xSWELLHALF \n
     - xWATT, xVA, xPF \n
     - ANGL_X_Y \n
     - xPeriod \n
     - STATUS0, STATUS1, STATUS2, STATUS3, ERROR_STATUS \n

     Following APIs are provided to convert these outputs
        - #adi_metic_ConvertPower
        - #adi_metic_ConvertRms
        - #adi_metic_ConvertEnergy
        - #adi_metic_ConvertPeriod
        - #adi_metic_ConvertAngle
        - #adi_metic_ConvertPowerFactor


     Registers can be periodically read by using IRQs from ADE9178. For example application can be read for every cycle,
     if #ADE9178_REG_MASK0 is configured to give interrupt for every RMSONERDY. If application is only interested in energy,
     EGYRDY bit to be enabled in #ADE9178_REG_MASK0. A sample code is given below. Refer to function
     #MetIcIfReadMetrologyParameters for more details

     @code

     while(irq0ready == 1)
     {
          irq0ready = 0
          adi_metic_ReadRegister(ADE9178_REG_STATUS0)
          adi_metic_WriteRegister(ADE9178_REG_STATUS0) // Clear the register to get next interrupt

          adi_metic_ReadRegister()  // Read all registers
          // Extract and Convert outputs

     }

     @endcode

     @subsection WFS Waveform Sample Collection.

     ADE9178 streams waveform through UART. Following APIs are provided to configure, start collecting the samples
     from Metrology IC. Note these API doesn't enable waveform streaming from metrology IC. That can be done
     writing to REG_WFS_CONFIG directly  using #adi_metic_WriteRegister API

     Following APIs can be used to configure , start and monitor waveform sample collection.

        - #adi_metic_WfsConfigureRx
        - #adi_metic_WfsStartRx

     As waveform streaming is asynchronous , it is not guaranteed to start with the first enabled channel.  One the waveform samples are collected. Following API can be used to find the offset
     of the the first sample of the channel present in the buffer.

        - #adi_metic_FindChannelOffset



    To see the full API reference you can either search for a specific API or
    browse by selecting the "Modules" node. The main APIs to be used are as
    follows.

    @subsection error_management Error Management
    Metrology Service APIs returns an enumeration of type #ADI_METIC_STATUS. If it returns an error of
    #ADI_METIC_STATUS_METIC_RETURNED_ERROR, then refer to Table 11: Status Code Description of ADE9178
    datasheet on how to handle the errors. Refer to @ref METICSTATUS for more details.

   @section Docs Documentation
   @subsection api_doc API document
   The Metrology Service (this document) is released in html
   generated from doxygen comments in the code


   @section Support Technical or Customer Support
   You can reach Analog Devices, Inc. Customer Support at:
        - E-mail Metrology service questions to
                 - energy.support@analog.com

   @section CopyrightIntro Copyright
    Copyright (c) 2023 - 2025 Analog Devices, Inc.

 */

/*
** EOF
*/

/*@}*/
