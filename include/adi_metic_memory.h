/******************************************************************************
 Copyright (c) 2022 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file
 * @brief       Defines memory required for Metrology Service library. This file
 * includes internal library defines to calculate memory size required. Include
 * this file only where #adi_metic_Create is called.
 * @{
 */

#ifndef __ADI_METIC_MEMORY_H__
#define __ADI_METIC_MEMORY_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** State memory required in bytes for the library. Allocate a buffer aligned
 * to 32 bit boundary */
#define ADI_METIC_STATE_MEM_NUM_BYTES 760

/** @} */
#ifdef __cplusplus
}
#endif

#endif /* __ADI_METIC_MEMORY_H__ */
