/******************************************************************************
 Copyright (c) 2024 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file  error_display.h
 * @brief       Definitions for handling and displaying error messages
 * @addtogroup    DISPLAY
 * @{
 */

#ifndef __ERROR_DISPLAY_H__
#define __ERROR_DISPLAY_H__

#include "adi_metic_status.h"
#include "metic_example.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Error status description of of ADE9178 */
extern char *ade9178ErrorCodes[];
/** Error status description of of ADE */
extern char *adeCmdErrorCodes[];
/** ERROR_STATUS register description */
extern char *errorStatusCodes[];

/**
 * @brief Displays Error Codes and the message of error code.
 * @param[in] address - address of the register.
 * @param[in] cmdType - specifies whether read or write is issued.
 * @param[in] regData - response from Metrology IC.
 * @param[in] errorCode -  error from Metrology IC
 */
void DisplayErrorCode(int32_t address, int32_t cmdType, int32_t regData,
                      ADI_METIC_STATUS errorCode);

/**
 * @brief Displays error message of ERROR_STATUS register.
 * @param[in] pExample -  pointer to example structure
 */
int32_t DisplayErrorStatusMessage(METIC_EXAMPLE *pExample);

/**
 * @brief Function to display Error Status Count
 */
void DisplayErrorStatusCount(void);

#ifdef __cplusplus
}
#endif

#endif /* __ERROR_DISPLAY_H__ */

/**
 * @}
 */
