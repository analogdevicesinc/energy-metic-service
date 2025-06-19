/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file adi_metic.h
 * @defgroup ADI_METIC Metrology Service APIs
 * @brief    Functions and Definations required to create instance, initialise service with the
 * given configurations, and to communicate with Metrology IC via SPI to get the metrology outputs
 * and converting into appropriate units.
 * @{
 */

#ifndef __ADI_METIC_H__
#define __ADI_METIC_H__

/*=============  I N C L U D E S   =============*/

#include "ade9178.h"
#include "adi_metic_status.h"
#include "metic_output_groups.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N E S =============*/

/** @defgroup    METICINIT Handle and Configurations
 * @brief This section contains the Configurations of Metrology Service.
 * The function pointers to be initialised from the application, this should be of same type
 * otherwise it throws warnings while building.
 * @{
 */

/** Maximum number of power channels */
#define ADI_METIC_MAX_NUM_POWER_CHANNELS 3
/** Maximum numer of channels */
#define ADI_METIC_MAX_NUM_CHANNELS 12
/** number of crc bytes  */
#define ADI_METIC_NUM_CRC_BYTES 2
/** read command  */
#define ADI_METIC_CMD_READ_REGISTER 0x01
/** write command  */
#define ADI_METIC_CMD_WRITE_REGISTER 0x00
/** Maximum number of registers can be read in a single call*/
#define ADI_METIC_MAX_NUM_REGISTERS (ADE9178_REG_CRC_RSLT - ADE9178_REG_AVRMS + 1)
/** @note Refer to ADE9178 datasheet for the fullscale codes and formats defined below*/
/** fullscale code for WATT and VA output*/
#define ADI_METIC_POWER_FS_CODE 85829040
/** fullscale code for RMS output*/
#define ADI_METIC_RMS_FS_CODE 107310840
/** fullscale code for PF output*/
#define ADI_METIC_PF_FS_CODE (1 << 27)
/** sampling rate Hz*/
#define ADI_METIC_SAMPLING_RATE 4000
/** Energy position of HI Register to combine both HI + LO*/
#define ADI_METIC_ENERGY_HI_POS 13
/** period format*/
#define ADI_METIC_PERIOD_FORMAT (1 << 16)
/** angle scale to downscale from 1024k to 4k to convert to radians*/
#define ADI_METIC_ANGLE_SCALE 256
/** iterations to check for finding channel offset in WFS*/
#define ADI_METIC_WFS_OFFSET_COUNT 8

/** Function pointer definition for SPI transmit */
typedef int32_t (*ADI_METIC_CMD_TRANSFER_FUNC)(void *, uint8_t *, uint32_t);
/** Function pointer definition for SPI receive */
typedef int32_t (*ADI_METIC_RESPONSE_RECEIVE_FUNC)(void *, uint8_t *, uint32_t);
/** Function pointer definition for UART receive */
typedef int32_t (*ADI_METIC_WFRM_RECEIVE_FUNC)(void *, uint8_t *, uint32_t);
/** Function pointer definition for setting UART baudrate */
typedef int32_t (*ADI_METIC_START_SETBAUDRATE_FUNC)(void *, uint32_t);
/** Function pointer definition for generic user defined functions */
typedef int32_t (*ADI_METIC_GENERIC_FUNC)(void *);
/** Function pointer definition for resume */
typedef int32_t (*ADI_METIC_WAIT_RESUME_FUNC)(void *, volatile uint8_t *);
/** Function pointer definition for suspend */
typedef int32_t (*ADI_METIC_WAIT_SUSPEND_FUNC)(void *, volatile uint8_t *);
/** Function pointer definition for generic user defined functions */
typedef int32_t (*ADI_METIC_EVENT_FUNC)(uint32_t, void *);
/** Function pointer definition for suspend */
typedef int32_t (*ADI_METIC_CRC_FUNC)(void *, uint8_t *, uint32_t);

/** A device handle used in all API functions to identify the instance.
 *  It is obtained from the #adi_metic_Create API. */
typedef void *ADI_METIC_HANDLE;

/**
 * Enumeration for the event type
 */
typedef enum
{
    /** Response is ready */
    ADI_METIC_MASTER_EVENT_TYPE_HOST_RDY,
    /** Error is ready to receive */
    ADI_METIC_MASTER_EVENT_TYPE_HOST_ERR,
    /** Response is completed*/
    ADI_METIC_MASTER_EVENT_TYPE_RESP_COMPLETED
} ADI_METIC_MASTER_EVENT_TYPE;

/**
 * @brief scomm service parameters
 */

#pragma pack(push, 1)
typedef struct
{
    /** read or write */
    uint32_t rwb : 1; // 1 - read, 0 - write
    /** device num */
    uint32_t device : 7;
    /** num of registers */
    uint32_t numRegisters : 8;
    /** adress */
    uint32_t addr : 16;
    /** data */
    int32_t data;
    /** crc */
    uint16_t crc;

} ADI_METIC_CMD;
#pragma pack(pop)

/**
 * Enumeration for Metrology Service Configurations
 */
typedef struct
{
    /** Function Pointer to start SPI transmssion */
    ADI_METIC_CMD_TRANSFER_FUNC pfCmdTransfer;
    /** Function Pointer to start SPI reception */
    ADI_METIC_RESPONSE_RECEIVE_FUNC pfResponseReceive;
    /** Function Pointer to start UART reception */
    ADI_METIC_WFRM_RECEIVE_FUNC pfWfrmReceive;
    /** Function Pointer to set UART baudrate*/
    ADI_METIC_START_SETBAUDRATE_FUNC pfSetBaudRate;
    /** Function Pointer to close */
    ADI_METIC_GENERIC_FUNC pfClose;
    /** Function Pointer to suspend state*/
    ADI_METIC_WAIT_SUSPEND_FUNC pfSuspend;
    /** Function Pointer to resume state*/
    ADI_METIC_WAIT_RESUME_FUNC pfResume;
    /** Function Pointer to add the CRC for the command packet*/
    ADI_METIC_CRC_FUNC pfAddCmdCrc;
    /** Function Pointer to verify the crc */
    ADI_METIC_CRC_FUNC pfVerifyRespCrc;
    /** user handle*/
    void *hUser;

} ADI_METIC_CONFIG;

/**
 * WFS Register Configurations.
 */
typedef struct
{
    /** Bit to indeicate WFS is enabled */
    unsigned int enable : 1;
    /** Baudrate selection of WFS */
    unsigned int baudrate : 4;
    /** output type selection WFS */
    unsigned int outputType : 2;
    /** Channels selection of WFS */
    unsigned int channelSelect : 12;
    /** Reserved bits for samplingRate */
    unsigned int samplingRate : 3;
    /** Reserved */
    unsigned int reserved : 10;

} ADI_METIC_WFS_ADE9178_REG_CONFIG;

/**
 * WFS Configurations.
 */
typedef struct
{
    /** WFS register configuration */
    ADI_METIC_WFS_ADE9178_REG_CONFIG wfsRegConfig;
    /** iteration count of verifying samples are synchronised */
    int32_t offsetCount;

} ADI_METIC_WFS_CONFIG;

/** @} */

/** @defgroup    METICOUTPUT Output Functions
 * @brief Functions and definations required to extract the set of registers and convert into
 *  appropriate units.
 *
 * @{
 */

/** @defgroup    METICOUTPUTSTRUCT Output Structures
 * @brief Structures are defined both in fixed (with suffix of _FIX) and floating
 * format as values read from the IC is in fixed format. This also contains set of output registers
 * to be used for extraction.
 *
 * @{
 */

/**
 * Structure to hold outputs after conversion
 */
typedef struct
{
    /** Stores outputs of all power parameters */
    ADI_METIC_POWER_OUTPUT powerOut[ADI_METIC_MAX_NUM_POWER_CHANNELS];
    /** Stores power factor output */
    float powerFactor[ADI_METIC_MAX_NUM_POWER_CHANNELS];
    /** Stores outputs of voltage channel RMS */
    ADI_METIC_RMS_OUTPUT rmsOut[ADI_METIC_MAX_NUM_CHANNELS];
    /** Stores outputs of period */
    ADI_METIC_PERIOD_OUTPUT periodOut;
    /** Stores outputs of angle */
    ADI_METIC_ANGLE_OUTPUT angleOut;
    /** Stores outputs of energy accumulation parameters */
    ADI_METIC_ENERGY_OUTPUT energyOut[ADI_METIC_MAX_NUM_POWER_CHANNELS];
    /** Stores outputs of status */
    ADI_METIC_STATUS_OUTPUT statusOut;

} ADI_METIC_OUTPUT;

/**
 * Structure to hold output register values
 */
typedef struct
{
    /** Stores outputs of all power parameters */
    ADI_METIC_POWER_OUTPUT_FIX powerOut[ADI_METIC_MAX_NUM_POWER_CHANNELS];
    /** Stores power factor output */
    int32_t powerFactor[ADI_METIC_MAX_NUM_POWER_CHANNELS];
    /** Stores outputs of all channel RMS */
    ADI_METIC_RMS_OUTPUT_FIX rmsOut[ADI_METIC_MAX_NUM_CHANNELS];
    /** Stores outputs of period */
    ADI_METIC_PERIOD_OUTPUT_FIX periodOut;
    /** Stores outputs of angle */
    ADI_METIC_ANGLE_OUTPUT_FIX angleOut;
    /** Stores outputs of energy  parameters */
    ADI_METIC_ENERGY_OUTPUT_FIX energyOut[ADI_METIC_MAX_NUM_POWER_CHANNELS];

} ADI_METIC_OUTPUT_FIX;

/** @} */

/** @} */

/** @defgroup   METICAPI IC Communication Functions
 * @brief This section covers API functions required to start and communicate with IC via SPI.
 * If waveform streaming is enabled, then collecting samples through UART. All the APIs return
 * enumeration codes in #ADI_METIC_STATUS. Refer to #ADI_METIC_STATUS for detailed documentation on
 * return codes
 * @{
 */

/**
 * @brief Function to create instance for Metrology Service.
 * Assigns memory and sets up the internal structures.ddd
 * of the library.
 * @param[out] phMetIc         Pointer to a location where the handle to
 * the service is written. This handle is required in all other service APIs.
 * @param[in]  pStateMemory          The pointer to the memory for the
 * library.
 * This pointer must be 32-bit aligned. This memory must be persistent
 * in the application. So it is recommended that it is not allocated in the
 * stack.
 * @param[in]  stateMemorySize  Size of the memory pointed by pStateMemory
 * This must be at least #ADI_METIC_STATE_MEM_NUM_BYTES bytes.
 * #adi_metic_SetConfig can also be used to set the configuration at a later point.
 * @return  #ADI_METIC_STATUS_SUCCESS\n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_INSUFFICIENT_STATE_MEMORY \n
 * #ADI_METIC_STATUS_SCOMM_INIT_ERROR
 */
ADI_METIC_STATUS adi_metic_Create(ADI_METIC_HANDLE *phMetIc, void *pStateMemory,
                                  uint32_t stateMemorySize);

/**
 * @brief Function to reconfigure metrology service.
 * @details    This API can be called whenever the user wants to reconfigure
 * service. This API will not reset the internal states of the service.
 * @param hMetIc   - Metrology service handle
 * @param pConfig - Pointer to the metrology service configuration.
 * @return  #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_SCOMM_INIT_ERROR
 */
ADI_METIC_STATUS adi_metic_SetConfig(ADI_METIC_HANDLE hMetIc, ADI_METIC_CONFIG *pConfig);

/**
 * @brief Initializes Metrology Service with the given configurations.
 * Before calling this API, its recommended to call #adi_metic_Create to create the instance and
 * also to populate configurations #ADI_METIC_CONFIG from application accordingly.
 * @param[in] hMetIc 		- Metrology service handle
 * @param pConfig - Pointer to the metrology service configuration.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_SCOMM_INIT_ERROR.
 */
ADI_METIC_STATUS adi_metic_Init(ADI_METIC_HANDLE hMetIc, ADI_METIC_CONFIG *pConfig);

/** @} */

/** @defgroup   METICCALLBACKAPI Callback Functions
 *  @brief This section covers callback APIs to be called from application ISR. All the APIs return
 * enumeration codes in #ADI_METIC_STATUS. Refer to #ADI_METIC_STATUS for detailed documentation on
 * return codes
 * @{
 */

/**
 * Callback for HOST_RDY pin. Based on the pinstate appropriate
 * events are set to indicate the ready to
 * send next command or the response is ready by suspending the wait state. This API should be
 * called from user callback.
 * @param[in] hMetIc - Metrology Service handle.
 * @param[in] pinState - pinstate of HOST_RDY.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR
 */
ADI_METIC_STATUS adi_metic_HostRdyCallback(ADI_METIC_HANDLE hMetIc, uint8_t pinState);

/**
 * Callback for HOST_ERR pin. Based on the pinstate appropriate
 * events are set to indicate the ready to
 * send next command or the response is ready by suspending the wait state. This API should be
 * called from user callback.
 * @param[in] hMetIc - Metrology Servie handle.
 * @param[in] pinState - pinstate of HOST_ERR.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR
 */
ADI_METIC_STATUS adi_metic_HostErrCallback(ADI_METIC_HANDLE hMetIc, uint8_t pinState);

/**
 * Callback for IRQ0 pin. This API is useful for monitoring the RST_DONE bit in
 * #ADE9178_REG_STATUS0. It should be called from user callback.
 * @param[in] hMetIc - Metrology Servie handle.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR
 */
ADI_METIC_STATUS adi_metic_Irq0Callback(ADI_METIC_HANDLE hMetIc);

/**
 * Callback for UART Rx completetion. This API sets the RX completion flag
 * internally. It should be called from user callback.
 * @param[in] hMetIc - Metrology Servie handle.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR
 */
ADI_METIC_STATUS adi_metic_WfsUartRxCallback(ADI_METIC_HANDLE hMetIc);

/** @} */

/**
 * @addtogroup METICAPI
 * @{
 */

/**
 * @brief Reads one or more METIC registers starting from address given.
 * This API send the command to IC and #ADI_METIC_CONFIG.pfSuspend makes the function to wait in a
 * loop. Whenever HOST_RDY/ HOST_ERR pin is detected ADI_METIC_CONFIG. pfResume function suspends
 * the waiting state in #adi_metic_HostRdyCallback / #adi_metic_HostErrCallback and returns
 * the response received from IC. \n
 * Sequence of steps are showed in below image.
 * \image html "reg_read_seq.png"
 * @param[in] hMetIc 		- Metrology service handle
 * @param[in] device   -  Device Id.
 *                         0 - ADE9178,
 *                         1 - ADC0,
 *                         2 - ADC1,
 *                         3 - ADC2,
 *                         4 - ADC3,
 *                         5 - ALL_ADC,
 * @param[in] addr      -  Address to read register
 * @param[in] numRegisters - Number of registers to read
 * @param[out] pValue      -  pointer to store register values
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_INVALID_NUM_REGISTERS \n
 * #ADI_METIC_STATUS_FRAME_CRC_ERROR \n
 * #ADI_METIC_STATUS_METIC_RETURNED_ERROR \n
 * #ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC \n
 * #ADI_METIC_STATUS_COMM_ERROR
 */
ADI_METIC_STATUS adi_metic_ReadRegister(ADI_METIC_HANDLE hMetIc, uint8_t device, uint16_t addr,
                                        uint32_t numRegisters, int32_t *pValue);
/**
 * @brief Write to METIC registers to address given.
 * This API send the command to IC and #ADI_METIC_CONFIG.pfSuspend makes the function to wait in a
 * loop. Whenever HOST_RDY/ HOST_ERR pin is detected ADI_METIC_CONFIG. pfResume function suspends
 * the waiting state in #adi_metic_HostRdyCallback / adi_metic_HostErrCallback and returns
 * the response received from IC.
 * Sequence of steps are showed in below image.
 * \image html "reg_write_seq.png"
 * @param[in] hMetIc 		- Metrology service handle
 * @param[in] device    -  Device Id.
 *                         0 - ADE9178,
 *                         1 - ADC0,
 *                         2 - ADC1,
 *                         3 - ADC2,
 *                         4 - ADC3,
 *                         5 - ALL_ADC,
 * @param[in] addr      -  Address to write register
 * @param[out] pValue      -  pointer to value of the register
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_FRAME_CRC_ERROR \n
 * #ADI_METIC_STATUS_METIC_RETURNED_ERROR \n
 * #ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC \n
 * #ADI_METIC_STATUS_COMM_ERROR
 */
ADI_METIC_STATUS adi_metic_WriteRegister(ADI_METIC_HANDLE hMetIc, uint8_t device, uint16_t addr,
                                         int32_t *pValue);

/**
 * @brief Function to terminates Metrology Service.
 * @param[in] hMetIc 		- Metrology service handle
 */
ADI_METIC_STATUS adi_metic_Close(ADI_METIC_HANDLE hMetIc);

/**
 * @brief Configures UART baudrate given in configuration only if
 * ADI_METIC_WFS_ADE9178_REG_CONFIG.enable = 1 and minimum of one channel to be enabled in
 * ADI_METIC_WFS_ADE9178_REG_CONFIG.channelSelect to collect samples. Copies configuration into
 * internal data to use this for other APIs. Refer to #ADE9178_REG_WFS_CONFIG for bitfield details.
 * @param[in] hMetIc - Metrology Servie handle.
 * @param[in] config - configuration of wfs.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_WFS_UART_BAUDRATE_ERROR.
 *
 */
ADI_METIC_STATUS adi_metic_WfsConfigureRx(ADI_METIC_HANDLE hMetIc, int32_t config);

/**
 * @brief Configures number of bytes to receive in UART DMA and starts the RX DMA only if
 * ADI_METIC_WFS_ADE9178_REG_CONFIG.enable = 1 and previous transaction is completed. Before calling
 * this API, #adi_metic_WfsConfigureRx to be called to set configurations.
 * @param[in] hMetIc - Metrology Servie handle.
 * @param[in] numBytes - number of bytes to receive.
 * @param[out] pSamples - Pointer to output samples.
 * @returns #ADI_METIC_STATUS_WFS_TRANSACTION_COMPLETED \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_WFS_UART_COMM_ERROR \n
 * #ADI_METIC_STATUS_WFS_PREV_TRANSACTION_IN_PROGRESS \n
 * #ADI_METIC_STATUS_WFS_DISABLED
 *
 */
ADI_METIC_STATUS adi_metic_WfsStartRx(ADI_METIC_HANDLE hMetIc, uint32_t numBytes,
                                      uint8_t *pSamples);

/**
 * @brief Finds the offset of the the first sample of the channel id given to be present in the
 * buffer. Format of data received from IC is in (data [MSB - 24 bits] + channel_id [LSB -8 bits]).
 * Refer to Waveform Streaming section in  ADE9178 data sheet for more details on transmission of
 * data. As waveform streaming is asynchronous , it is not guaranteed to start with the first
 * enabled channel. This API searches for channel id in given samples. Once it finds first sample,
 * it iterates for 8 times to decide that rest of the samples has all the enabled channels. This API
 * is recommended to call after #adi_metic_WfsStartRx.
 * @param[in] hMetIc - Metrology Servie handle.
 * @param[in]  pSamples - Pointer to input samples.
 * @param[in]  numBytes - number of bytes.
 * @param[in]  channelId - channel id to find offset of the the first sample in input samples.
 * @param[out]  pByteOffset - offset in bytes.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NO_VALID_SAMPLES \n
 * #ADI_METIC_STATUS_NULL_PTR
 *
 */
ADI_METIC_STATUS adi_metic_FindChannelOffset(ADI_METIC_HANDLE hMetIc, int8_t *pSamples,
                                             int32_t numBytes, int32_t channelId,
                                             int32_t *pByteOffset);

/**
 * Checks integrity errors and startup errors, and then starts ADC and ADE9178 by enabling RUN and
 * INIT Bit of #ADE9178_REG_ADC_CONTROL. It's recommended to call
 * after resetting board.
 * @param[in] hMetIc - Metrology Servie handle.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_IRQ0_NOT_RECEIVED \n
 * #ADI_METIC_STATUS_IC_INTEGRITY_ERROR \n
 * #ADI_METIC_STATUS_STARTUP_ERROR \n
 * #ADI_METIC_STATUS_STARTUP_PENDING \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_FRAME_CRC_ERROR \n
 * #ADI_METIC_STATUS_METIC_RETURNED_ERROR \n
 * #ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC
 */
ADI_METIC_STATUS adi_metic_StartAdc(ADI_METIC_HANDLE hMetIc);

/**
 * Checks Integrity Error, any startup errors occurred at boot up time. It's recommended to call
 * after resetting board.
 * @param[in] hMetIc - Metrology Servie handle.
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_IC_INTEGRITY_ERROR \n
 * #ADI_METIC_STATUS_STARTUP_ERROR \n
 * #ADI_METIC_STATUS_IRQ0_NOT_RECEIVED \n
 * #ADI_METIC_STATUS_STARTUP_PENDING \n
 * #ADI_METIC_STATUS_FRAME_CRC_ERROR \n
 * #ADI_METIC_STATUS_METIC_RETURNED_ERROR \n
 * #ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC
 */
ADI_METIC_STATUS adi_metic_CheckStartupStatus(ADI_METIC_HANDLE hMetIc);

/** @} */

/**
 * @addtogroup METICOUTPUT
 * @{
 */

/** @defgroup   METICOUTPUTCONVERT Output Extraction and Conversion
 * @brief Functions and definations required to extract the set of registers and convert into
 *  appropriate units.
 *
 * @{
 */

/**
 * @brief Extracts register values from source based on valid index given and
 * stores register values in destination buffer. This API can be useful to extract grouped registers
 * of specific types from all the output registers for the conversion. \n
 * Below are the list of grouped registers
 *      - #statusRegisters \n
 *      - #rmsRegisters \n
 *      - #rmsRegisters \n
 *      - #powerFactorRegisters \n
 *      - #powerRegisters \n
 *      - #periodRegisters \n
 *      - #energyRegisters \n
 *      - #angleRegisters. \n
 * @param[in] pSrc      -  pointer to register values
 * @param[in] pIndex      -  pointer to valid indices which corresponds to position of the selected
 * register to extract.
 * @param[in] numRegisters      - number of registers in index buffer.
 * @param[out] pDst      -  pointer to output buffer buffer after extraction.
 */
void adi_metic_ExtractRegisters(int32_t *pSrc, uint32_t *pIndex, uint32_t numRegisters,
                                int32_t *pDst);
/**
 * \image html "power_eqn.png"
 * @brief Converts the active, apparent power output register values into
 * appropriate units. Refer to Table 15 in datasheet for conversion.
 * @param[in] pSrc      -  pointer to power registers.
 * @param[in] numRegisters      -  number of registers.
 * @param[out] pDst      -  pointer to converted power registers.
 */
void adi_metic_ConvertPower(ADI_METIC_POWER_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                            ADI_METIC_POWER_OUTPUT *pDst);

/**
 * \image html "powerfactor_eqn.png"
 * @brief Converts the power factor output register values into appropriate
 * units. Refer to Table 15 in datasheet for conversion.
 * @param[in] pSrc      -  pointer to power factor registers.
 * @param[in] numRegisters      -  number of registers.
 * @param[out] pDst      -  pointer to converted power factor registers.
 */
void adi_metic_ConvertPowerFactor(int32_t *pSrc, uint32_t numRegisters, float *pDst);

/**
 * \image html "rms_eqn.png"
 * @brief Converts rms output register values into appropriate
 * units. Refer to Table 15 in datasheet for conversion.
 * @param[in] pSrc      -  pointer to channel rms registers.
 * @param[in] numRegisters      -  number of registers.
 * @param[out] pDst      -  pointer to converted channel rms registers.
 */
void adi_metic_ConvertRms(ADI_METIC_RMS_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                          ADI_METIC_RMS_OUTPUT *pDst);

/**
 * \image html "energy_eqn.png"
 * @brief Converts the energy output register values into watt seconds.
 * @param[in] pSrc      -  pointer to energy accumulation registers.
 * @param[in] numRegisters      -  number of registers.
 * @param[out] pDst      -  pointer to converted energy accumulation registers.
 */
void adi_metic_ConvertEnergy(ADI_METIC_ENERGY_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                             ADI_METIC_ENERGY_OUTPUT *pDst);

/**
 * \image html "period_eqn.png"
 * @brief Converts the period output register values into seconds.
 * @param[in] pSrc      -  pointer to period registers.
 * @param[in] numRegisters      -  number of registers.
 * @param[out] pDst      -  pointer to converted period registers.
 */
void adi_metic_ConvertPeriod(ADI_METIC_PERIOD_OUTPUT_FIX *pSrc, uint32_t numRegisters,
                             ADI_METIC_PERIOD_OUTPUT *pDst);

/**
 * \image html "angle_eqn.png"
 * @brief Converts the angle output register values into radians. Refer to Table
 * 15 in datasheet for conversion.
 * @param[in] pAngleSrc      -  pointer to angle registers.
 * @param[in] pPeriodSrc      -  pointer to period registers.
 * @param[in] numRegisters      -  number of registers.
 * @param[out] pDst      -  pointer to converted angle registers.
 */
void adi_metic_ConvertAngle(ADI_METIC_ANGLE_OUTPUT_FIX *pAngleSrc, int32_t *pPeriodSrc,
                            uint32_t numRegisters, ADI_METIC_ANGLE_OUTPUT *pDst);

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __ADI_METIC_H__ */

/**
 * @}
 */
