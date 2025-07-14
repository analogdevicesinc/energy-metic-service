/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @addtogroup MET_EXAMPLE
 * @{
 */

/**
 * @file cli_commands.h
 * @brief       Definitions for cli commands
 * @defgroup    CLI_COMMANDS cli
 * @{
 */

#ifndef __CLI_COMMANDS_H__
#define __CLI_COMMANDS_H__

/*============= I N C L U D E S =============*/
#include "cli_interface.h"
#include "cli_utils.h"
#include "metic_example.h"
#include <complex.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
/*============= D E F I N E S =============*/

/** Maximum number of parameters that can be passed */
#define MAX_PARAMETER_COUNT 8

/*============= F U N C T I O N  P R O T O T Y P E S =============*/
/**
 * @brief Function for CLI "start" command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdStart(Args *pArgs);

/**
 * @brief Function for CLI "reset" command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdReset(Args *pArgs);

/**
 * @brief Function for CLI version command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdVersion(Args *pArgs);
/**
 * @brief Function for CLI "setdisplay" command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdSetDisplay(Args *pArgs);

/**
 * @brief Function for CLI "getdisplay" command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdGetDisplay(Args *pArgs);

/**
 * @brief Function for CLI "setconfig" command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */

int32_t CmdSetConfig(Args *pArgs);

/**
 * @brief Function for CLI "getconfig" command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdGetConfig(Args *pArgs);

/**
 * @brief Function to setreg command  to write registers
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdSetRegData(Args *pArgs);

/**
 * @brief Function to getreg command to read registers
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdGetRegData(Args *pArgs);
/**
 * @brief Function to getpulsetime command to get pulse times.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdDisplayPulseTime(Args *pArgs);

/**
 * @brief Function for CLI "display" description
 */
void DescDisplay(void);

/**
 * @brief Function for CLI "config" description
 */
void DescConfig(void);

/**
 * @brief Function to load registers value of ade9178
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdLoadReg(Args *pArgs);

/**
 * @brief Function to save registers value in flash
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdSaveReg(Args *pArgs);

/**
 * @brief Function for CLI Capture Waveform command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdCaptureWfrm(Args *pArgs);

/**
 * @brief Function for CLI Dispalay waveform command.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdDisplayWfrm(Args *pArgs);

/**
 * @brief Function for CLI Dispalay of Error Status count.
 * @param[in] pArgs - Pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CmdDisplayErrorStatusCount(Args *pArgs);

/**
 * @brief Function for CLI "close" command.
 * @param[in] pArgs       - pointer to command arguments storage
 * @return status 0 on Success
 */
int32_t CliClose(Args *pArgs);

#ifdef __cplusplus
}
#endif

#endif /* __CLI_COMMANDS_H__ */

/**
 * @}
 */

/**
 * @}
 */
