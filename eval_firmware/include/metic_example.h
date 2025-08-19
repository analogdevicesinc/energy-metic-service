/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file metic_example.h
 * @addtogroup MET_EXAMPLE Example
 * @{
 */

/**
 * @brief       Definitions for intialising, setting configurations and running the example
 * @defgroup    METIC_EXAMPLE Ade Example
 * @{
 */

#ifndef __METIC_EXAMPLE_H__
#define __METIC_EXAMPLE_H__

/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "adi_nvm.h"
#include "app_cfg.h"
#include "cli_service_interface.h"
#include "metic_config_groups.h"
#include "metic_output_groups.h"
#include "metic_service_interface.h"
#include "nvm_service_interface.h"
#include "pulse_count.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#if APP_CFG_ENABLE_RTOS == 1
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** Max number of outputs can be read from ADE9178. */
#define MAX_NUM_OTUPUTS_PER_CYCLE 140
/** Max number of status outputs can be read from ADE9178. */
#define MAX_NUM_STATUS_OUTPUTS_PER_CYCLE 12
/**  Macro for met service cli thread flag */
#define MET_SERVICE_CLI_THREAD_FLAG 0x1
/**  Macro for met service readmet thread flag */
#define MET_SERVICE_READ_MET_THREAD_FLAG 0x1
/** Supporting rtos feature*/
#define ADI_CFG_USE_RTOS 0
/** Resets only ADE9178*/
#define RESET_DEVICE_ADE9178 1
/** Resets only ADC*/
#define RESET_DEVICE_ADC 2
/** Resets both ADC and ADE9178*/
#define RESET_DEVICE_ALL 3

/**
 * @brief ADE Example states
 *
 */
typedef enum
{
    /** Code is waiting for a start command*/
    METIC_EXAMPLE_STATE_WAITING_FOR_START_CMD,
    /** ADE Example is running*/
    METIC_EXAMPLE_STATE_RUNNING
} METIC_EXAMPLE_STATE;

/**
 * @brief ADE Example configuration enum
 *
 */
typedef enum
{
    /** index of config voltage scale */
    METIC_EXAMPLE_CONFIG_VOLTAGE_SCALE = 1,
    /** index of config current scale */
    METIC_EXAMPLE_CONFIG_CURRENT_SCALE,
    /** index of aux voltage scale */
    METIC_EXAMPLE_CONFIG_AUX_SCALE
} METIC_EXAMPLE_CONFIG_SCALE;

/**
 * @brief Display Configurations
 *
 */
typedef struct
{
    /*! Enable all display */
    bool enableAll;
    /*! Enable rms output */
    bool enableRmsOutput;
    /*! Enable rmsone output */
    bool enableRmsOneOutput;
    /*! Enable rmshalf output */
    bool enableRmsHalfOutput;
    /*! Enable dip and swell channel rmsone Output */
    bool enableEventRmsOneOutput;
    /*! Enable dip and swell channel rmshalf Output */
    bool enableEventRmsHalfOutput;
    /*! Enable power output */
    bool enablePowerOutput;
    /*! Enable energy output */
    bool enableEnergyOutput;
    /*! Enable period. */
    bool enablePeriodOutput;
    /*! Enable angle. */
    bool enableAngleOutput;
    /*! Enable status output */
    bool enableStatusOutput;
    /*! Enable Error status output */
    bool enableErrorStatusOutput;
    /*! voltage scaling factor */
    float voltageScale;
    /*! current scaling factor */
    float currentScale;
    /*! auxiliary scaling factor */
    float auxScale;
} ADE_DISPLAY_CONFIG;

/**
 * Structure for ADE example configurations.
 */
typedef struct
{
    /** Configuration for displaying output **/
    ADE_DISPLAY_CONFIG displayConfig;
    /** Number of test cycles */
    uint32_t cyclesToRun;
    /** Configuration of wfs */
    int32_t wfsConfig;
    /** enables registers read */
    int32_t enableRegisterRead;
    /** enables to start waveform capture */
    int32_t enableWfrmCapture;
} METIC_EXAMPLE_CONFIG;

/**
 * Structure to hold calibration parameters required
 * for channel processing
 */
typedef struct
{
    /** Gain value */
    int32_t gain;
    /** Offset calibration value */
    int32_t offset;
    /** Stores beta values required for phase compensation */
    int32_t phaseCal;
    /** Offset value used in filtered RMS calculation */
    int32_t rmsOffset;
    /** Offset value used in rms half cycle calculation */
    int32_t rmsHalfOffset;
    /** Offset value used in rms linecycle calculation */
    int32_t rmsOneOffset;
} METIC_EXAMPLE_CHANNEL_CAL_COEFFS;

/**
 * Structure to hold calibration parameters required
 * for power calculation
 */
typedef struct
{
    /** Gain value used in power calculation */
    int32_t powerGain;
    /** Offset value used in active power calculation */
    int32_t wattOffset;
} METIC_EXAMPLE_POWER_CAL_COEFFS;

/**
 * Structure to hold calibration parameters
 */
typedef struct
{
    /** Stores calibration coefficient value of channel*/
    METIC_EXAMPLE_CHANNEL_CAL_COEFFS channelCalCoeffs[ADI_METIC_MAX_NUM_CHANNELS];
    /** Stores calibration coefficient value of power*/
    METIC_EXAMPLE_POWER_CAL_COEFFS powerCalCoeffs[ADI_METIC_MAX_NUM_POWER_CHANNELS];
} METIC_EXAMPLE_CAL_COEFFS;

/**
 * Structure to hold calibration configuration registers value
 */
typedef struct
{
    /** Stores configuration parameters of CONFIG0 register */
    uint32_t config0;
    /** Stores CF1 configuration */
    uint32_t cf1Config;
    /** Stores CF2 configuration */
    uint32_t cf2Config;
    /** Stores Nominal phase voltage RMS */
    uint32_t vNom;
    /** Stores user defined period.*/
    uint32_t userPeriod;
    /** Stores energy threshold for generating CF1 pulse */
    uint32_t cf1Thrsh;
    /** Stores  energy threshold for generating CF2 pulse */
    uint32_t cf2Thrsh;
    /** Stores zcr and line period related configurations */
    uint32_t zxLpSel;
    /** Stores threshold value for voltage zero crossing.*/
    uint32_t zxThrsh;
    /** Stores configuration of active power noload level */
    uint32_t actPwrNoLoadLvl;
    /** Stores configuration of apparent power noload level */
    int32_t appPwrNoLoadLvl;
    /** Stores energy time*/
    uint32_t energyTime;
    /** Stores energy accumulation configuration */
    uint32_t epConfig;
    /** ADC Datapath Redirection Register 0 */
    uint32_t adcRedirect0;
    /** ADC Datapath Redirection Register 1 */
    uint32_t adcRedirect1;
    /** adc config register */
    uint32_t adcConfig;
    /** CF Calibration Pulse Width Configuration Register */
    uint32_t cfLtmr;
} METIC_EXAMPLE_ADE9178_REG_CONFIG;

/**
 * Structure to hold status registers value
 */
typedef struct
{
    /** Stores MASK0 register */
    uint32_t mask0;
    /** Stores MASK1 register */
    uint32_t mask1;
    /** Stores MASK2 register */
    uint32_t mask2;
    /** Stores MASK3 register */
    uint32_t mask3;
    /** Stores ERROR_MASK register*/
    uint32_t errorMask;
} METIC_EXAMPLE_STATUS_ADE9178_REG_CONFIG;

/**
 * Structure to hold configuration registers value
 */
typedef struct
{
    /** Stores calibration coefficient register value */
    METIC_EXAMPLE_CAL_COEFFS calCoeff;
    /** Stores calibration coefficient register value */
    METIC_EXAMPLE_ADE9178_REG_CONFIG regConfig;
    /** Stores status register value */
    METIC_EXAMPLE_STATUS_ADE9178_REG_CONFIG statusConfig;
} NVM_REG;

/**
 * Structure for nvm
 */
typedef struct
{
    /** configuration registers */
    NVM_REG reg;
} METIC_EXAMPLE_NVM;

/**
 * Structure for ADE9178 configurations.
 */
typedef struct
{
    /** configuration registers */
    ADE_CHANNEL_CONFIG channelConfig[ADI_METIC_MAX_NUM_CHANNELS];
    /** configuration registers */
    ADE_POWER_CONFIG powerConfig[ADI_METIC_MAX_NUM_POWER_CHANNELS];
    /** configuration registers */
    ADE_COMMON_CONFIG commConfig;
    /** vsumrmone offset configuration register */
    METIC_REGISTER vsumrmsoneos;
    /** vsum swell one configuration register */
    METIC_REGISTER vsumswellonelvl;
    /** vsum level configuration register */
    METIC_REGISTER vsumswellonecyc;
    /** penfault configuration register */
    ADE_PEN_CONFIG penConfig[ADI_METIC_MAX_NUM_POWER_CHANNELS];
    /** CF Calibration Pulse Width Configuration Register */
    METIC_REGISTER cfLtmr;
    /** wfs config registers */
    METIC_REGISTER wfsConfig;
    /** adc control register */
    METIC_REGISTER adcControl;
    /** adc config register */
    METIC_REGISTER adcConfig;
    /** ADC Datapath Redirection Register 0 */
    METIC_REGISTER adcRedirect0;
    /** ADC Datapath Redirection Register 1 */
    METIC_REGISTER adcRedirect1;
    /** adc config register */
    METIC_REGISTER crcOpten;
    /** adc config register */
    METIC_REGISTER crcForce;
    /** adc config register */
    METIC_REGISTER configLock;
    /** status configuration registers */
    ADE_STATUS_CONFIG statusConfig;
} ADE_CONFIG_REG;

#if APP_CFG_ENABLE_RTOS == 1
/**
 * Structure for ADE example thread information.
 */
typedef struct
{
    /** id for metOutput thread */
    TaskHandle_t metOutputThread;
    /** id for metOutput thread */
    TaskHandle_t cmdThread;
} ADE_THREAD_INFO;
#endif

/**
 *
 * Structure for ADE example.
 */
typedef struct
{
    /** Example configuration */
    METIC_EXAMPLE_CONFIG exampleConfig;
    /** WFS register configuration */
    ADI_METIC_WFS_ADE9178_REG_CONFIG wfsRegConfig;
    /** Example states*/
    METIC_EXAMPLE_STATE state;
    /** processed cycles */
    uint32_t processedCycles;
    /** ade9178 configuration structure */
    ADE_CONFIG_REG adeConfig;
    /** register output structure */
    ADI_METIC_OUTPUT output;
    /** Stores to register value with crc into flash*/
    METIC_EXAMPLE_NVM nvm;
    /** SCOMM instance structure*/
    METIC_INSTANCE_INFO adeInstance;
    /** Board Handler */
    void *hEvb;
    /** Board configurations */
    ADI_EVB_CONFIG evbConfig;
    /** previous status of ERROR_STATUS register*/
    int32_t prevErrorStatus;
    /** Data required to interface cli service */
    CLI_INTERFACE_INFO cliIf;
    /** Data required to interface nvm service */
    NVM_INTERFACE_INFO nvmIf;
    /** Pulse counter information to keep count of CFs*/
    PCNT_INFO pcntInfo;
    /** Buffer for pulse counter */
    uint32_t pcntBuffer[APP_CFG_MAX_NUM_IRQ_TIME];

} METIC_EXAMPLE;

/** registers list with respect to NVM_REG */
extern METIC_REGISTER adeExampleNvmReg[];

/*============= D E F I N I T I O N S =============*/

/**
 * @brief Initializes board
 *
 * @return 0 - Success - 1 Failed
 */
int32_t InitBoard(void);

/**
 * @brief Initialzies CLI, Metrology servcies needed for example
 *
 * @return 0 - Success - 1 Failed
 */
int32_t InitServices(void);

/**
 * @brief Loads calibration values from flash into Metrology IC if present, enables RMSONE_RDY
 * interrupt mode and intialises example states.
 *
 * @return 0 - Success - 1 Failed
 */
int32_t InitExampleStates(void);

/**
 * @brief Initializes threads
 *
 * @return 0 - Success - 1 Failed
 */
int32_t InitThreads(void);

/**
 * @brief Get ADE Example structure
 *
 * @return pointer to METIC_EXAMPLE structure
 */
METIC_EXAMPLE *GetAdeExample(void);

/**
 * @brief Collects Metrology outputs
 *
 * @return 0 - Success - 1 Failed
 */
int32_t CollectMetOutputs(void);

/**
 * @brief Notify Output Thread
 *
 */
void NotifyMetOutputThread(void);

/**
 * @brief Function to get the command and call the appropriate dispatch function APIs.
 *
 */
void CliExample(int32_t status);

/**
 * @brief Get CLI Interface handle
 *
 * @return pointer to CLI_INTERFACE structure
 */
ADI_CLI_HANDLE GetCliHandle(void);

/**
 * @brief Runs the example in loop
 *
 * @return 0 - Success - 1 Failed
 */
int32_t RunExample(void);

/**
 * @brief Handles run state of the example
 *
 * @return 0 - Success
 */
int32_t HandleRunState(void);

/**
 * @brief Resets Metrology IC based on device, starts ADC, and enables RMSONE_RDY interrupt
 * mode.
 * @param[in] device	  - device to reset
 * @return 0 - Success
 */
int32_t HandleResetCmd(int32_t device);

/**
 * @brief Resets IRQ status and handles start command
 * @return 0 - Success
 */
int32_t HandleStartCmd(void);

/**
 * @brief Handles savereg command
 * @return 0 - Success
 */
int32_t HandleSaveRegCmd(void);

/**
 * @brief Handles loadreg command and displays values.
 * @return 0 - Success
 */
int32_t HandleLoadRegCmd(void);

/**
 * @brief Handles setconfig command
 * @param[in] configChoice	  - choice to configure scale
 * @param[in] value	  - value to configure
 */
void HandleSetConfigCmd(uint16_t configChoice, float value);

/**
 * @brief Handles getpulsetime command
 * @param[in] pulseId	  - Id to display
 */
void HandleDisplayPulseTimeCmd(uint32_t pulseId);

/**
 * @brief Loads values into flash
 * @param[in] pExample	  - pointer to example structure.
 * @return 0 - Success
 */
int32_t LoadNvmReg(METIC_EXAMPLE *pExample);

/**
 * @brief saves values into flash
 * @param[in] pExample	  - pointer to example structure.
 * @return 0 - Success
 */
int32_t SaveNvmReg(METIC_EXAMPLE *pExample);

/**
 * @brief Function to return pointer of example configuration structure.
 * @return pointer to the example configuration structure.
 */
METIC_EXAMPLE_CONFIG *GetExampleConfig(void);

/**
 * @brief Gets Metrology Service Instance
 */
METIC_INSTANCE_INFO *GetAdeInstance(void);

#ifdef __cplusplus
}
#endif

#endif /* __METIC_EXAMPLE_H__ */

/**
 * @}
 */

/**
 * @}
 */
