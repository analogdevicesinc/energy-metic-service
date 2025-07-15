/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file         metic_config_groups.h
 * @brief        List of ADE9178 configuration registers
 * @addtogroup ADI_ADE
 * @{
 */
#ifndef __METIC_CONFIG_GROUPS_H__
#define __METIC_CONFIG_GROUPS_H__

/*=============  I N C L U D E S   =============*/

#include "ade9178.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N E S =============*/

/** Maximum number of registers for configuration and status registers*/
#define MAX_NUM_REGISTERS                                                                          \
    (ADE9178_REG_CONFIG_LOCK - ADE9178_REG_AVGAIN) +                                               \
        (ADE9178_REG_ERROR_MASK - ADE9178_REG_STATUS0) + 2
/**
 * Structure for configurations
 */
typedef struct
{
    /** address of register*/
    uint32_t address;
    /** value of register*/
    int32_t value;
} METIC_REGISTER;

/**
 * Structure for configurations
 */
typedef struct
{
    /** address of register*/
    uint32_t address;
    /** value of register*/
    uint32_t value;
} ADE_REGISTER_UINT;

/**
 * Structure to hold configuration registers value
 */
typedef struct
{
    /** Stores configuration parameters of CONFIG0 register */
    METIC_REGISTER config0;
    /** Stores configurations for peak */
    METIC_REGISTER peakConfig;
    /** Stores configurations for penfault */
    METIC_REGISTER penConfig;
    /** Stores configurations for CF1 configuration */
    METIC_REGISTER cf1Config;
    /** Stores configurations for CF2 configuration */
    METIC_REGISTER cf2Config;
    /** Stores configurations for ISUMRMS configuration */
    METIC_REGISTER isumRmsOffset;
    /** Stores configurations for ISUMLVL configuration */
    METIC_REGISTER isumLvl;
    /** Nominal phase voltage RMS */
    METIC_REGISTER vNom;
    /** Stores user defined period.*/
    METIC_REGISTER userPeriod;
    /** Stores rmshalf configuration. */
    METIC_REGISTER rmsHalfConfig;
    /** Stores user defined half period. */
    METIC_REGISTER userPeriodHalfConfig;
    /** Energy threshold for CF1 */
    METIC_REGISTER cf1Thrsh;
    /** Energy threshold for CF2 */
    METIC_REGISTER cf2Thrsh;
    /** Stores zcr timeout configurations */
    METIC_REGISTER zxTimeout;
    /** Threshold value for voltage zero crossing.*/
    METIC_REGISTER zxThrsh;
    /** zerocrossing line period select.*/
    METIC_REGISTER zxLpSel;
    /** sequence cycles.*/
    METIC_REGISTER seqCyc;
    /** No load threshold for total and fundamental active power */
    METIC_REGISTER actPwrNoLoadLvl;
    /** No load threshold for total and fundamental apparent power */
    METIC_REGISTER appPwrNoLoadLvl;
    /** Stores energy accumulation configuration parameters */
    METIC_REGISTER epConfig;
    /** Stores time (number of samples) for energy accumulation*/
    METIC_REGISTER energyTime;

} ADE_COMMON_CONFIG;

/**
 * Structure for channel configurations
 */
typedef struct
{
    /** Stores gain */
    METIC_REGISTER gain;
    /** Stores offset */
    METIC_REGISTER offset;
    /** Stores phasecal */
    METIC_REGISTER phcal;
    /** Stores rms offset */
    METIC_REGISTER rmsoffset;
    /** Stores rmshalf offset */
    METIC_REGISTER rmshalfoffset;
    /** Stores rmsone offset */
    METIC_REGISTER rmsoneoffset;
    /** Stores diphalf level */
    METIC_REGISTER diphalflvl;
    /** Stores dipone level */
    METIC_REGISTER diponelvl;
    /** Stores swellhalf level */
    METIC_REGISTER swellhalflvl;
    /** Stores swellone level */
    METIC_REGISTER swellonelvl;
    /** Stores diphalf cycle */
    METIC_REGISTER diphalfcyc;
    /** Stores dipone cycle */
    METIC_REGISTER diponecyc;
    /** Stores swellhalf cycle */
    METIC_REGISTER swellhalfcyc;
    /** Stores swellone cycle */
    METIC_REGISTER swellonecyc;

} ADE_CHANNEL_CONFIG;

/**
 * Structure to hold status registers value
 */
typedef struct
{
    /** Stores STATUS0 register */
    METIC_REGISTER status0;
    /** Stores STATUS1 register */
    METIC_REGISTER status1;
    /** Stores STATUS2 register */
    METIC_REGISTER status2;
    /** Stores STATUS3 register */
    METIC_REGISTER status3;
    /** Stores ERROR_STATUS register */
    METIC_REGISTER errorStatus;
    /** Stores MASK0 register */
    METIC_REGISTER mask0;
    /** Stores MASK1 register */
    METIC_REGISTER mask1;
    /** Stores MASK2 register */
    METIC_REGISTER mask2;
    /** Stores MASK3 register */
    METIC_REGISTER mask3;
    /** Stores ERROR_MASK register*/
    METIC_REGISTER errorMask;
} ADE_STATUS_CONFIG;

/**
 * Structure for pen configurations
 */
typedef struct
{
    /** Stores rmsone offset */
    METIC_REGISTER rmsoneos;
    /** Stores dipone level */
    METIC_REGISTER diponelvl;
    /** Stores dipone cycle */
    METIC_REGISTER diponecyc;
    /** Stores swellone level */
    METIC_REGISTER swellonelvl;
    /** Stores swellone cycle */
    METIC_REGISTER swellonecyc;

} ADE_PEN_CONFIG;

/**
 * Structure for power configurations
 */
typedef struct
{
    /** Stores gain */
    METIC_REGISTER gain;
    /** Stores offset */
    METIC_REGISTER offset;

} ADE_POWER_CONFIG;

extern ADE_REGISTER_UINT configReg[MAX_NUM_REGISTERS];

#ifdef __cplusplus
}
#endif

#endif /* __METIC_CONFIG_GROUPS_H__ */

/**
 * @}
 */
