/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file      app_cfg.h
 * @brief     This file contains configurations required for Metrology Service
 * @defgroup EXAMPLE_CFG Configurations
 * @{
 */

#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/*=============  I N C L U D E S   =============*/

#ifdef __cplusplus
extern "C" {
#endif

/*=============  D E F I N I T I O N S  =============*/

/** Maximum timeout count to send command and get response from Metrology IC */
#define APP_CFG_TIMEOUT_COUNT 10000000
/** SPI interrupt priority*/
#define APP_CFG_ADE9178_SPI_INT_PRIORITY 4
/** SPI TX interrupt priority */
#define APP_CFG_ADE9178_SPI_TX_INT_PRIORITY 4
/** SPI RX interrupt priority */
#define APP_CFG_ADE9178_SPI_RX_INT_PRIORITY 4
/** SPI speed */
#define APP_CFG_ADE9178_SPI_SPEED 10000000
/** GPIO interrupt priority */
#define APP_CFG_PORT0_GPIO_INT_PRIO 5
/** SPI mode */
#define APP_CFG_ADE9178_SPI_MODE 3
/** Enable SPI communication to send and receive commands */
#define APP_CFG_ENABLE_ADE9178_SPI 1
/** Enable SPI communication to send and receive commands */
#define APP_CFG_ENABLE_GPIO 1
/** macro to enable host uart*/
#define APP_CFG_ENABLE_HOST_UART 1
/** UART Tx/Rx interrupt priority for host*/
#define APP_CFG_UART_INTR_PRIORITY 4
/** host uart baudrate*/
#define APP_CFG_HOST_UART_SPEED 115200
/** configuration value of ZXTHRSH to be set at boot time*/
#define APP_CFG_ADE9178_ZXTHRSH 0xe14c
/** configuration value of CF1_CONFI to be set at boot time*/
#define APP_CFG_ADE9178_CF1_CONFIG 0x70
/** configuration value of CF1_THR to be set at boot time*/
#define APP_CFG_ADE9178_CF1_THR 0xebce17
/** configuration value of EP_CFG to be set at boot time*/
#define APP_CFG_ADE9178_EP_CFG 0x15
/** configuration value of EGY_TIME to be set at boot time*/
#define APP_CFG_ADE9178_EGY_TIME 0X3e7
/** Timeout(usec) to wait till startup is done */
#define APP_CFG_STARTUP_TIMEOUT_COUNT 16000
/** configuration value of CONFIG0 to be set at boot time*/
#define APP_CFG_ADE9178_CONFIG0 0xd90
/** maximum number of irq time to store in the buffer*/
#define APP_CFG_MAX_NUM_IRQ_TIME 2048
/** Enable Timer */
#define APP_CFG_ENABLE_SYSTEM_TIMER 1

#ifdef __cplusplus
}
#endif

#endif /* __APP_CFG_H__ */

/**
 * @}
 */
