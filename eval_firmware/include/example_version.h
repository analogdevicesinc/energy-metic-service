/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file
 * @brief     Defines for release versions
 * @{
 */

#ifndef __EXAMPLE_VERSION_H__
#define __EXAMPLE_VERSION_H__

/*============= I N C L U D E S =============*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=============  D E F I N I T I O N S  =============*/

/** Product name */
#define ADE9178_EXAMPLE_NAME "ADE9178 Command Line Interface(CLI)"
/** define for major revision*/
#define ADE9178_EXAMPLE_MAJOR_REVISION 1
/** define for minor revision*/
#define ADE9178_EXAMPLE_MINOR_REVISION 1
/** define for patch number*/
#define ADE9178_EXAMPLE_PATCH_NUMBER 0
/** Id to differentiate different builds of same release.
 *  Updated through  automation*/
#define ADE9178_EXAMPLE_BUILD_HASH 0x7fd58aa9
/** Build Number.
 *  Updated through  automation*/
#define ADE9178_EXAMPLE_BUILD_NUM 2507140

/** Release version */
#define ADE9178_EXAMPLE_VERSION                                                                    \
    ((ADE9178_EXAMPLE_MAJOR_REVISION << 24) | (ADE9178_EXAMPLE_MINOR_REVISION << 16) |             \
     (ADE9178_EXAMPLE_PATCH_NUMBER << 8))

#ifdef __cplusplus
}
#endif

#endif /* __EXAMPLE_VERSION_H__ */

/**
 * @}
 */
