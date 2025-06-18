/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file         metic_output_groups.h
 * @addtogroup METICOUTPUTSTRUCT
 * @{
 */
#ifndef __METIC_OUTPUT_GROUPS_H__
#define __METIC_OUTPUT_GROUPS_H__

/*=============  I N C L U D E S   =============*/

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============= D E F I N E S =============*/

/** List of POWER registers */
extern uint32_t powerRegisters[6];
/** List of powerfactor registers*/
extern uint32_t powerFactorRegisters[3];
/** List of Voltage RMS registers*/
extern uint32_t rmsRegisters[84];
/** List of ENERGY registers*/
extern uint32_t energyRegisters[24];
/** List of PERIOD registers*/
extern uint32_t periodRegisters[4];
/** List of ANGLE registers*/
extern uint32_t angleRegisters[9];
/** List of STATUS registers*/
extern uint32_t statusRegisters[5];

/**
 * Structure to hold power outputs after conversion
 */

typedef struct
{
    /** Stores total active power output */
    float activePower;
    /** Stores total apparent power output */
    float apparentPower;

} ADI_METIC_POWER_OUTPUT;

/**
 * Structure to hold power register values
 */

typedef struct
{
    /** Stores total active power output */
    int32_t activePower;
    /** Stores total apparent power output */
    int32_t apparentPower;

} ADI_METIC_POWER_OUTPUT_FIX;

/**
 * Structure to hold rms outputs after conversion
 */
typedef struct
{
    /** Stores filtered rms value */
    float filteredRms;
    /** Stores rmsone cycle value */
    float rmsOneCycle;
    /** Stores rmshalf cycle value */
    float rmsHalfCycle;
    /** Stores dipone rms value */
    float dipOne;
    /** Stores diphalf rms value */
    float dipHalf;
    /** Stores swellone rms value */
    float swellOne;
    /** Stores swellhalf rms value */
    float swellHalf;
} ADI_METIC_RMS_OUTPUT;

/**
 * Structure to hold rms register values
 */
typedef struct
{
    /** Stores filtered rms value */
    int32_t filteredRms;
    /** Stores rmsone cycle value */
    int32_t rmsOneCycle;
    /** Stores rmshalf cycle value */
    int32_t rmsHalfCycle;
    /** Stores dipone rms value */
    int32_t dipOne;
    /** Stores diphalf rms value */
    int32_t dipHalf;
    /** Stores swellone rms value */
    int32_t swellOne;
    /** Stores swellhalf rms value */
    int32_t swellHalf;
} ADI_METIC_RMS_OUTPUT_FIX;

/**
 * Structure to hold energy outputs after conversion
 */

typedef struct
{
    /** Stores accumulated positive total active energy value*/
    float posActEgy;
    /** Stores accumulated negative total active energy value*/
    float negActEgy;
    /** Stores accumulated signed accumulated total active energy value*/
    float signActEgy;
    /** Stores accumulated total apparent energy*/
    float apparentEgy;
} ADI_METIC_ENERGY_OUTPUT;

/**
 * Structure to hold energy register values
 */

typedef struct
{
    /** Stores positive total active energy LSB value*/
    int32_t posActEgyLo;
    /** Stores positive total active energy MSB value*/
    int32_t posActEgyHi;
    /** Stores negative total active energy LSB value*/
    int32_t negActEgyLo;
    /** Stores negative total active energy MSB  value*/
    int32_t negActEgyHi;
    /** Stores signed total active energy LSB value*/
    int32_t signActEgyLo;
    /** Stores signed total active energy MSB value*/
    int32_t signActEgyHi;
    /** Stores total apparent energy LSB value*/
    int32_t apparentEgyLo;
    /** Stores total apparent energy MSB value*/
    int32_t apparentEgyHi;
} ADI_METIC_ENERGY_OUTPUT_FIX;

/**
 * Structure to hold status register values
 */

typedef struct
{
    /** Stores status0 value */
    uint32_t status0;
    /** Stores status1 value */
    uint32_t status1;
    /** Stores status2 value */
    uint32_t status2;
    /** Stores status3 value */
    uint32_t status3;
    /**Stores  errorstatus value */
    uint32_t errorStatus;

} ADI_METIC_STATUS_OUTPUT;

/**
 * Structure to hold period output after conversion
 */

typedef struct
{
    /** Stores phaseA period value */
    float aPeriod;
    /** Stores phaseB period value */
    float bPeriod;
    /** Stores phaseC period value */
    float cPeriod;
    /** Stores combined period value */
    float comPeriod;

} ADI_METIC_PERIOD_OUTPUT;

/**
 * Structure to hold period register values
 */

typedef struct
{
    /** Stores phaseA period value */
    int32_t aPeriod;
    /** Stores phaseB period value */
    int32_t bPeriod;
    /** Stores phaseC period value */
    int32_t cPeriod;
    /** Stores combined period value */
    int32_t comPeriod;

} ADI_METIC_PERIOD_OUTPUT_FIX;

/**
 * Structure to hold angle outputs after conversion
 */

typedef struct
{
    /** Stores angle between Va and Vb channels */
    float angl_av_bv;
    /** Stores angle between Vb and Vc channels */
    float angl_bv_cv;
    /** Stores angle between Va and Vc channels */
    float angl_av_cv;
    /** Stores angle between Va and Ia channels */
    float angl_av_ai;
    /** Stores angle between Vb and Ib channels */
    float angl_bv_bi;
    /** Stores angle between Vc and Ic channels */
    float angl_cv_ci;
    /** Stores angle between Ia and Ib channels */
    float angl_ai_bi;
    /** Stores angle between Ib and Ic channels */
    float angl_bi_ci;
    /** Stores angle between Ia and Ic channels */
    float angl_ai_ci;

} ADI_METIC_ANGLE_OUTPUT;

/**
 * Structure to hold angle register values
 */

typedef struct
{
    /** Stores angle between Va and Vb channels */
    int32_t angl_av_bv;
    /** Stores angle between Vb and Vc channels */
    int32_t angl_bv_cv;
    /** Stores angle between Va and Vc channels */
    int32_t angl_av_cv;
    /** Stores angle between Va and Ia channels */
    int32_t angl_av_ai;
    /** Stores angle between Vb and Ib channels */
    int32_t angl_bv_bi;
    /** Stores angle between Vc and Ic channels */
    int32_t angl_cv_ci;
    /** Stores angle between Ia and Ib channels */
    int32_t angl_ai_bi;
    /** Stores angle between Ib and Ic channels */
    int32_t angl_bi_ci;
    /** Stores angle between Ia and Ic channels */
    int32_t angl_ai_ci;

} ADI_METIC_ANGLE_OUTPUT_FIX;

#ifdef __cplusplus
}
#endif

#endif /* __METIC_OUTPUT_GROUPS_H__ */

/**
 * @}
 */
