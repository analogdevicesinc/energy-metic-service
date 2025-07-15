/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file example_display.h
 * @addtogroup EXAMPLE
 * @{
 */

/**
 * @file        example_display.h
 * @brief       Definitions for displaying outputs and configurations
 * @defgroup    DISPLAY display
 * @{
 */

#ifndef __EXAMPLE_DISPLAY_H__
#define __EXAMPLE_DISPLAY_H__

/*============= I N C L U D E S =============*/

#include "metic_example.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N I T I O N S =============*/

/**
 * @brief Function to display metrology status outputs
 * @param[in] pDisplay -  pointer to display
 * @param[in] irqCount -  irq count
 * @param[in] pOutput -  pointer to output.
 */
void DisplayOutput(ADE_DISPLAY_CONFIG *pDisplay, uint32_t irqCount, ADI_METIC_OUTPUT *pOutput);

/**
 * @brief Function to display configuration value
 * @param[in] configChoice -  configuration choice
 */
void DisplayConfigValue(uint16_t configChoice);

/**
 * @brief Function to display list of nvm regiser values
 * @param[in] pConfig -  pointer to ade configuration structure.
 */
void DisplayNvmReg(ADE_CONFIG_REG *pConfig);

/**
 * @brief Displays Waveform Samples for 100ms.
 */
void DisplayWaveformOutput(void);

/**
 * @brief Function to display burst output registers
 * @param[in] addr -  address of register
 * @param[in] numRegisters -  number of registers to display
 * @param[in] pRegData -  pointer to register data
 */
void DisplayBurstRegisters(uint32_t addr, uint32_t numRegisters, int32_t *pRegData);

/**
 * @brief Function to intialise display configurations
 * @param[in] pConfig -  pointer to display configuration structure.
 */
void InitDisplayConfig(ADE_DISPLAY_CONFIG *pConfig);

#ifdef __cplusplus
}
#endif

#endif /* __EXAMPLE_DISPLAY_H__ */
/**
 * @}
 */

/**
 * @}
 */
