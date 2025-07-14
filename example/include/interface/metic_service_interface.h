/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_service_interface.h
 * @defgroup    METIC_INTERFACE Interface Layer to Metrology Service
 * @brief      	Interface Layer for initialising Metrology Service, handling user callbacks and
 * functions required by the application to get metrology outputs by calling appropriate Service
 * APIs.
 * @{
 */

#ifndef __METIC_SERVICE_INTERFACE_H__
#define __METIC_SERVICE_INTERFACE_H__

/*============= I N C L U D E S =============*/

#include "adi_metic.h"
#include "adi_metic_memory.h"
#include "app_cfg.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup  METICINTERFACEPROCESS
 * @{
 */

/** @defgroup    METICINTERFACECONFIG Macros
 * @brief This section contains macros needed for running the application.
 * @{
 */

/** Maximum message size per cycle*/
#define MAX_MSG_STORAGE_SIZE_PER_CYCLE 3806
/** size of error count buffer */
#define ERROR_COUNT_BUFFER_SIZE ADE9178_BITP_ERROR_STATUS_ERROR7 + 1
/** Macro to enable streaming 12 channels*/
#define ENABLE_ALL_CHANNELS 1
#if ENABLE_ALL_CHANNELS == 1
/** number of samples to store for 100msec with all channels enabled. */
#define WFS_NUM_SAMPLES 4800
/** Size of WFS buffer */
#define WFS_BUFFER_SIZE 8000
#else
/** number of samples to store for 500msec with 6 channels enabled. */
#define WFS_NUM_SAMPLES 12000
/** Size of WFS buffer */
#define WFS_BUFFER_SIZE 16000
#endif

/** @} */
/** @} */

/** @defgroup    ADEINTERFACEINITCONFIG Configurations and User Data
 * @brief This section contains the configurations required to initialise Metrology Service and user
 * data required by the application to process, display or monitor.
 * @{
 */

/**
 * Structure to hold IRQ status.
 */
typedef struct
{
    /** IRQ0 flag */
    volatile uint8_t irq0Ready;
    /** IRQ1 flag */
    volatile uint8_t irq1Ready;
    /** IRQ2 flag */
    volatile uint8_t irq2Ready;
    /** IRQ3 flag */
    volatile uint8_t irq3Ready;
    /** IRQ0 count */
    volatile uint32_t irq0Count;
    /** stores time of last IRQ0 */
    volatile uint32_t lastIrqTime;

} ADE_IRQ_STATUS;

/**
 * Structure to hold data for user handle.
 */
typedef struct
{
    /** Handle for Metrology Service */
    ADI_METIC_HANDLE hAde;
    /** Configuration of Metrology Service */
    ADI_METIC_CONFIG config;
    /** Library memory for Metrology Service */
    int32_t stateMemory[ADI_METIC_STATE_MEM_NUM_BYTES / 4];
    /** integrity status */
    int32_t integrityStatus;
    /** WFS Rx Completion status */
    int32_t isWfsRxComplete;
    /** Irq status*/
    ADE_IRQ_STATUS irqStatus;
    /** Buffer for pulse counter */
    uint32_t cfTime[APP_CFG_MAX_NUM_IRQ_TIME];
    /** Buffer for pulse counter */
    uint32_t cfFlag[APP_CFG_MAX_NUM_IRQ_TIME];
    /** index of the number of IRQs stored in the buffer */
    uint32_t cfIndex;
    /** Enable regsiter read */
    uint8_t enableRegisterRead;
    /** Enable WFS capture */
    uint8_t enableWfsCapture;
    /** register output structure in fix format */
    ADI_METIC_OUTPUT_FIX outputFix;
    /** output register buffer */
    int32_t regBuffer[ADI_METIC_MAX_NUM_REGISTERS];
    /** status register buffer */
    int32_t regStatusBuffer[ADI_METIC_MAX_NUM_REGISTERS];
    /** Buffer to store wfs sammples */
    int32_t wfsBuffer[WFS_BUFFER_SIZE];
    /** stores error status count */
    int32_t errorStatusCount[ERROR_COUNT_BUFFER_SIZE];

} METIC_INSTANCE_INFO;

/** @} */

/** @defgroup    ADEINTERFACEINIT Metrology IC Init Functions
 * @brief Functions required to create Metrology Service instance and initialise with
 * appropriate configurations by calling service APIs to start IC.
 * @{
 */

/**
 * @brief Creates instance for Metrology Service. And initialises after populating the
 * configurations in #ADI_METIC_CONFIG required for SPI, UART communication. It supports pulse
 * counter feature to monitor CF pulses.
 * @param[in] pInfo 		- User instance Handle
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_SCOMM_INIT_ERROR.
 */
ADI_METIC_STATUS MetIcIfCreateInstance(METIC_INSTANCE_INFO *pInfo);

/**
 * @brief Callback for GPIO to handle the HOST_RDY, HOST_ERR, IRQ0, IRQ1, IRQ2, IRQ3, CF1, CF2
 * pins based on the flag received and call the appropriate Metrology Service APIs.
 * @param[in]  port  - GPIO port
 * @param[in]  flag  - status of GPIO
 */
void MetIcIfGPIOCallback(uint32_t port, uint32_t flag);

/**
 * @brief Callback for WFS Uart. After all sample receiving completed, it calls
 * #adi_metic_WfsUartRxCallback.
 */
void MetIcIfWfsUartCallBack(void);

/**
 * @brief Function to terminate Metroloogy Service. This function mainly used
 * in simulation mode.
 * @param[in] pInfo 		- User instance
 */
void MetIcIfClose(METIC_INSTANCE_INFO *pInfo);

/**
 * @brief Function to check startup errors, configures Metrology IC with mentioned
 * registers \n #ADE9178_REG_CONFIG0, #ADE9178_REG_ZXTHRSH, #ADE9178_REG_CF1_CONFIG,
 * #ADE9178_REG_CF1_THR, #ADE9178_REG_EP_CFG, #ADE9178_REG_EGY_TIME to user defined at startup time
 * and starts ADC. Its recommended to reset the IC before calling this.
 * @param[in] pInfo 		- User instance
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_IRQ0_NOT_RECEIVED \n
 * #ADI_METIC_STATUS_IC_INTEGRITY_ERROR \n
 * #ADI_METIC_STATUS_STARTUP_ERROR \n
 * #ADI_METIC_STATUS_STARTUP_PENDING \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_FRAME_CRC_ERROR \n
 * #ADI_METIC_STATUS_METIC_RETURNED_ERROR \n
 * #ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC
 *
 */
ADI_METIC_STATUS MetIcIfStartAdc(METIC_INSTANCE_INFO *pInfo);

/**
 * @brief Function to handle multiple IRQs and call the appropriate service API callbacka and set
 * the user status flags based on gpio flag received.
 * @param[in] flag 		- status of GPIO pin
 *
 */
void MetIcIfUpdateIrqStatus(uint32_t flag);

/** @} */

/** @defgroup    ADEINTERFACEPROCESS Process Functions
 * @brief These functions are useful to get all metrology outputs, and convert set of output
 * registers into real units, and monitoring the errors occurred during the process.
 * @{
 */

/**
 * @brief Function to start reading all metrology outputs by calling #adi_metic_ReadRegister with
 * appropriate number of registers and converting the outputs that are in fixed format into
 * appropriate units. Reading of outputs is done at every RMSONERDY.
 * @param[in] pInfo 		- User instance
 * @param[in] pOutput 		- pointer to output to store read values from Metrology IC.
 * @returns 0 on success
 *
 */
int32_t MetIcIfReadMetrologyParameters(METIC_INSTANCE_INFO *pInfo, ADI_METIC_OUTPUT *pOutput);

/**
 * @brief Function to reset #ADE_IRQ_STATUS. It's useful when data collection to be started newly.
 * @param[in] pInfo 		- User instance
 *
 */
ADI_METIC_STATUS MetIcIfResetIrqStatus(METIC_INSTANCE_INFO *pInfo);

/**
 * @brief Function to monitor IRQ1 pin to check whether errors occurred during the process. If the
 * errors occurred, then #ADE9178_REG_ERROR_STATUS is to be cleared by W1/C. And also
 * #ADE9178_REG_STATUS1 to be cleared by W1/C to get the next interrupt when occurs.
 * @param[in] pInfo 		- User instance
 * @param[out] pErrorRegStatus 		- pointer to status of ERROR_STATUS register
 *
 */
ADI_METIC_STATUS MetIcIfGetErrorStatus(METIC_INSTANCE_INFO *pInfo, int32_t *pErrorRegStatus);

/**
 * @brief Function to clear ADCx_STATUSx errors by writing 1 to registers.
 * @param[in] pInfo 		- User instance
 * @param[out] errorRegStatus 		- value of ERROR_STATUS register
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_INVALID_NUM_REGISTERS \n
 * #ADI_METIC_STATUS_FRAME_CRC_ERROR \n
 * #ADI_METIC_STATUS_METIC_RETURNED_ERROR \n
 * #ADI_METIC_STATUS_NO_RESPONSE_FROM_METIC
 *
 */
ADI_METIC_STATUS MetIcIfClearAdcStatusError(METIC_INSTANCE_INFO *pInfo, int32_t errorRegStatus);

/**
 * @brief Function to configure Rx, and starting Rx DMA to collect samples.
 * @param[in] pInfo 		- User instance
 * @param[in] config 		- configuration of WFS
 * @returns #ADI_METIC_STATUS_SUCCESS \n
 * #ADI_METIC_STATUS_NULL_PTR \n
 * #ADI_METIC_STATUS_WFS_UART_BAUDRATE_ERROR \n
 * #ADI_METIC_STATUS_WFS_TRANSACTION_COMPLETED \n
 * #ADI_METIC_STATUS_WFS_UART_COMM_ERROR \n
 * #ADI_METIC_STATUS_WFS_PREV_TRANSACTION_IN_PROGRESS \n
 * #ADI_METIC_STATUS_WFS_DISABLED
 *
 */
ADI_METIC_STATUS MetIcIfCollectSamples(METIC_INSTANCE_INFO *pInfo, int32_t config);

/**
 * @brief Suspends the (non os) thread by going into a wait state.
 * Times out if suspend state has not changed within timeout metioned in app_cfg.h file.
 * #ADI_METIC_CONFIG.pfSuspend to be initialised with this function.
 * @param[in]  pInfo  - User Handle
 * @param[in]  pSuspendState  - pointer to suspend state variable.
 * @return  timeout or success
 */
int32_t MetIcIfSuspend(void *pInfo, volatile uint8_t *pSuspendState);

/**
 * @brief Resumes the thread. #ADI_METIC_CONFIG.pfResume to be initialised with this function.
 *
 * @param[in]  pInfo  - User instance info
 * @param[in]  pSuspendState  - pointer to suspend state variable.
 * @return  error if user info is NULL, else success
 */
int32_t MetIcIfResume(void *pInfo, volatile uint8_t *pSuspendState);

/**
 * @brief Function to populate the configurations required for Metrology Service.
 * @param[in] pConfig 		- pointer to configuration
 *
 */
void MetIcAdaptPopulateConfig(ADI_METIC_CONFIG *pConfig);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __METIC_SERVICE_INTERFACE_H__ */

/**
 * @}
 */
