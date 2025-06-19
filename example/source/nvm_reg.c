/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file 	nvm_reg.c
 * @brief       Definitions needed to save registers value into flash or
 *							load registers value from flash.
 * @{
 */
/*==========================  I N C L U D E S   ==========================*/
#include "ade9178.h"
#include "adi_crc_ccitt16.h"
#include "adi_nvm.h"
#include "error_display.h"
#include "message.h"
#include "metic_config_groups.h"
#include "metic_example.h"
#include "metic_service_interface.h"
#include <stdint.h>
#include <string.h>

/** Register list with respect to #NVM_REG */
/** Note: If any new element is added to this list, then NVM_REG struct should also be updated*/
METIC_REGISTER adeExampleNvmReg[] = {
    {ADE9178_REG_AVGAIN, 0},        {ADE9178_REG_AVOS, 0},          {ADE9178_REG_AVPHCAL, 0},
    {ADE9178_REG_AVRMSOS, 0},       {ADE9178_REG_AVRMSHALFOS, 0},   {ADE9178_REG_AVRMSONEOS, 0},
    {ADE9178_REG_AIGAIN, 0},        {ADE9178_REG_AIOS, 0},          {ADE9178_REG_AIPHCAL, 0},
    {ADE9178_REG_AIRMSOS, 0},       {ADE9178_REG_AIRMSHALFOS, 0},   {ADE9178_REG_AIRMSONEOS, 0},
    {ADE9178_REG_BVGAIN, 0},        {ADE9178_REG_BVOS, 0},          {ADE9178_REG_BVPHCAL, 0},
    {ADE9178_REG_BVRMSOS, 0},       {ADE9178_REG_BVRMSHALFOS, 0},   {ADE9178_REG_BVRMSONEOS, 0},
    {ADE9178_REG_BIGAIN, 0},        {ADE9178_REG_BIOS, 0},          {ADE9178_REG_BIPHCAL, 0},
    {ADE9178_REG_BIRMSOS, 0},       {ADE9178_REG_BIRMSHALFOS, 0},   {ADE9178_REG_BIRMSONEOS, 0},
    {ADE9178_REG_CVGAIN, 0},        {ADE9178_REG_CVOS, 0},          {ADE9178_REG_CVPHCAL, 0},
    {ADE9178_REG_CVRMSOS, 0},       {ADE9178_REG_CVRMSHALFOS, 0},   {ADE9178_REG_CVRMSONEOS, 0},
    {ADE9178_REG_CIGAIN, 0},        {ADE9178_REG_CIOS, 0},          {ADE9178_REG_CIPHCAL, 0},
    {ADE9178_REG_CIRMSOS, 0},       {ADE9178_REG_CIRMSHALFOS, 0},   {ADE9178_REG_CIRMSONEOS, 0},
    {ADE9178_REG_AUX0GAIN, 0},      {ADE9178_REG_AUX0OS, 0},        {ADE9178_REG_AUX0PHCAL, 0},
    {ADE9178_REG_AUX0RMSOS, 0},     {ADE9178_REG_AUX0RMSHALFOS, 0}, {ADE9178_REG_AUX0RMSONEOS, 0},
    {ADE9178_REG_AUX1GAIN, 0},      {ADE9178_REG_AUX1OS, 0},        {ADE9178_REG_AUX1PHCAL, 0},
    {ADE9178_REG_AUX1RMSOS, 0},     {ADE9178_REG_AUX1RMSHALFOS, 0}, {ADE9178_REG_AUX1RMSONEOS, 0},
    {ADE9178_REG_AUX2GAIN, 0},      {ADE9178_REG_AUX2OS, 0},        {ADE9178_REG_AUX2PHCAL, 0},
    {ADE9178_REG_AUX2RMSOS, 0},     {ADE9178_REG_AUX2RMSHALFOS, 0}, {ADE9178_REG_AUX2RMSONEOS, 0},
    {ADE9178_REG_AUX3GAIN, 0},      {ADE9178_REG_AUX3OS, 0},        {ADE9178_REG_AUX3PHCAL, 0},
    {ADE9178_REG_AUX3RMSOS, 0},     {ADE9178_REG_AUX3RMSHALFOS, 0}, {ADE9178_REG_AUX3RMSONEOS, 0},
    {ADE9178_REG_AUX4GAIN, 0},      {ADE9178_REG_AUX4OS, 0},        {ADE9178_REG_AUX4PHCAL, 0},
    {ADE9178_REG_AUX4RMSOS, 0},     {ADE9178_REG_AUX4RMSHALFOS, 0}, {ADE9178_REG_AUX4RMSONEOS, 0},
    {ADE9178_REG_AUX5GAIN, 0},      {ADE9178_REG_AUX5OS, 0},        {ADE9178_REG_AUX5PHCAL, 0},
    {ADE9178_REG_AUX5RMSOS, 0},     {ADE9178_REG_AUX5RMSHALFOS, 0}, {ADE9178_REG_AUX5RMSONEOS, 0},
    {ADE9178_REG_APGAIN, 0},        {ADE9178_REG_AWATTOS, 0},       {ADE9178_REG_BPGAIN, 0},
    {ADE9178_REG_BWATTOS, 0},       {ADE9178_REG_CPGAIN, 0},        {ADE9178_REG_CWATTOS, 0},
    {ADE9178_REG_CONFIG0, 0},       {ADE9178_REG_CF1_CONFIG, 0},    {ADE9178_REG_CF2_CONFIG, 0},
    {ADE9178_REG_VNOM, 0},          {ADE9178_REG_USER_PERIOD, 0},   {ADE9178_REG_CF1_THR, 0},
    {ADE9178_REG_CF2_THR, 0},       {ADE9178_REG_ZXTHRSH, 0},       {ADE9178_REG_ZX_LP_SEL, 0},
    {ADE9178_REG_ACT_NL_LVL, 0},    {ADE9178_REG_APP_NL_LVL, 0},    {ADE9178_REG_EP_CFG, 0},
    {ADE9178_REG_EGY_TIME, 0},      {ADE9178_REG_CF_LTMR, 0},       {ADE9178_REG_ADC_CONFIG, 0},
    {ADE9178_REG_ADC_REDIRECT0, 0}, {ADE9178_REG_ADC_REDIRECT1, 0}, {ADE9178_REG_MASK0, 0},
    {ADE9178_REG_MASK1, 0},         {ADE9178_REG_MASK2, 0},         {ADE9178_REG_MASK3, 0},
    {ADE9178_REG_ERROR_MASK, 0}};

/**
 * @brief Reads values from flash and verifies crc.
 * @param[out] pNvm   - pointer to nvm.
 * @returns 0 on successful reading from flash.
 */
static int32_t ReadFromFlash(ADI_NVM_HANDLE hNvm, METIC_EXAMPLE_NVM *pNvm);

/**
 * @brief Calculates crc, and writes values to flash.
 * @param[out] pNvm   - pointer to nvm.
 * @returns 0 on successful writing to flash.
 */

static int32_t WritetoFlash(ADI_NVM_HANDLE hNvm, METIC_EXAMPLE_NVM *pNvm);

/**
 * @brief Copies pSrc to pDst, and writes register values to Metrology IC.
 * @param[in] pSrc - pointer to source.
 * @param[in] numSrcRegisters   -  number of registers in pSrc.
 * @param[in] numDstRegisters   -  number of registers in pDst.
 * @param[out] pDst   -  pointer to Destination.
 * @returns 0 on successeful writing to Metrology IC.
 */
static int32_t WriteConfigRegister(METIC_REGISTER *pSrc, int32_t numSrcRegisters,
                                   int32_t numDstRegisters, METIC_REGISTER *pDst);

/**
 * @brief Reads register values from Metrology IC, and Copies pSrc to pDst.
 * @param[in] pSrc - pointer to source.
 * @param[in] numSrcRegisters   -  number of registers in pSrc.
 * @param[in] numDstRegisters   -  number of registers in pDst.
 * @param[out] pDst   -  pointer to Destination.
 * @returns 0 on successeful writing to Metrology IC.
 */
static int32_t ReadConfigRegister(METIC_REGISTER *pSrc, int32_t numSrcRegisters,
                                  int32_t numDstRegisters, METIC_REGISTER *pDst);
/**
 * @brief sends write command to Metrology IC.
 * @param[in] address - address of register
 * @param[in] pValue   - pointer to value to be written.
 * @returns 0 on success
 */
static int32_t WriteToSlave(uint32_t address, int32_t *pValue);
/**
 * @brief sends read command to Metrology IC.
 * @param[in] address - address of register
 * @param[in] pValue   - pointer to store register values.
 * @returns 0 on success
 */
static int32_t ReadFromSlave(uint32_t address, int32_t *pValue);

/**
 * @brief Copies values from pSrc of type (address, value) into pDst.
 * @param[in] pSrc - pointer to source.
 * @param[in] numRegisters   - number of registers.
 * @param[out] pDst   -  pointer to Destination.
 * @returns 0 on success
 */
static void CopyNvmAddressToNvm(METIC_REGISTER *pSrc, int32_t numRegisters, int32_t *pDst);

/**
 * @brief Copies values from pSrc into pDst of type (address, value).
 * @param[in] pSrc - pointer to source.
 * @param[in] numRegisters   - number of registers.
 * @param[out] pDst   -  pointer to Destination.
 * @returns 0 on success
 */
static void CopyNvmToNvmAddress(int32_t *pSrc, int32_t numRegisters, METIC_REGISTER *pDst);

static int32_t GetRegIndex(uint32_t address, int32_t numRegisters, METIC_REGISTER *pDst);

int32_t LoadNvmReg(METIC_EXAMPLE *pExample)
{
    int32_t status = 0;
    int32_t numSrcRegisters;
    int32_t numDstRegisters;
    int32_t *pNvmConfig = (int32_t *)&pExample->nvm.reg;
    METIC_REGISTER *pConfig = (METIC_REGISTER *)&pExample->adeConfig;
    status = ReadFromFlash(pExample->nvmIf.hNvm, &pExample->nvm);
    if (status == 0)
    {

        numSrcRegisters = sizeof(adeExampleNvmReg) / sizeof(adeExampleNvmReg[0]);
        numDstRegisters = sizeof(configReg) / sizeof(configReg[0]);
        CopyNvmToNvmAddress(pNvmConfig, numSrcRegisters, &adeExampleNvmReg[0]);
        status =
            WriteConfigRegister(&adeExampleNvmReg[0], numSrcRegisters, numDstRegisters, pConfig);
        if (status == 0)
        {
            INFO_MSG("Loaded config/calibration registers from Flash to ADE9178.")
        }
    }
    return status;
}

int32_t SaveNvmReg(METIC_EXAMPLE *pExample)
{
    int32_t status = 0;
    int32_t numSrcRegisters;
    int32_t numDstRegisters;
    int32_t *pNvmConfig = (int32_t *)&pExample->nvm.reg;
    METIC_REGISTER *pConfig = (METIC_REGISTER *)&pExample->adeConfig;
    numDstRegisters = sizeof(adeExampleNvmReg) / sizeof(adeExampleNvmReg[0]);
    numSrcRegisters = sizeof(configReg) / sizeof(configReg[0]);
    status = ReadConfigRegister(pConfig, numSrcRegisters, numDstRegisters, &adeExampleNvmReg[0]);
    CopyNvmAddressToNvm(&adeExampleNvmReg[0], numDstRegisters, pNvmConfig);
    if (status == 0)
    {
        status = WritetoFlash(pExample->nvmIf.hNvm, &pExample->nvm);
    }
    return status;
}

static int32_t WritetoFlash(ADI_NVM_HANDLE hNvm, METIC_EXAMPLE_NVM *pNvm)
{
    ADI_NVM_STATUS nvmStatus;
    uint32_t crcValue;
    int32_t status = 0;
    NVM_REG *pReg = &pNvm->reg;
    crcValue = adi_crc_CalculateCCITT16((uint8_t *)pReg, sizeof(NVM_REG));
    pNvm->crc = crcValue;
    nvmStatus = adi_nvm_Write(hNvm, (uint8_t *)pNvm, 0, sizeof(METIC_EXAMPLE_NVM));
    if (nvmStatus != ADI_NVM_STATUS_SUCCESS)
    {
        WARN_MSG("NVM write failed")
        status = -1;
    }
    return status;
}

static int32_t ReadFromFlash(ADI_NVM_HANDLE hNvm, METIC_EXAMPLE_NVM *pNvm)
{
    ADI_NVM_STATUS nvmStatus;
    uint32_t crcValue;
    int32_t status = 0;
    NVM_REG *pReg = &pNvm->reg;
    nvmStatus = adi_nvm_Read(hNvm, 0, sizeof(METIC_EXAMPLE_NVM), (uint8_t *)pNvm);
    if (nvmStatus == ADI_NVM_STATUS_SUCCESS)
    {
        crcValue = adi_crc_CalculateCCITT16((uint8_t *)pReg, sizeof(NVM_REG));
        if (crcValue != pNvm->crc)
        {
            WARN_MSG("No valid config/calibration registers found in Flash.")
            status = 1;
        }
    }
    else
    {
        WARN_MSG("NVM Read Failed.")
        status = -1;
    }
    return status;
}

static int32_t WriteConfigRegister(METIC_REGISTER *pSrc, int32_t numSrcRegisters,
                                   int32_t numDstRegisters, METIC_REGISTER *pDst)
{
    int32_t status = 0;
    int32_t index;
    int32_t i;
    // Iterates through list of registers in #adeExampleNvmReg, gets index of the address present in
    // destination buffer (ADE_CONFIG_REG) and updates the value in that index location and send
    // write command to slave.
    for (i = 0; i < numSrcRegisters; i++)
    {
        index = GetRegIndex(pSrc[i].address, numDstRegisters, pDst);
        if (index != -1)
        {
            memcpy(&pDst[index].value, &pSrc[i].value, 4);
            status = WriteToSlave(pDst[index].address, &pDst[index].value);
            if (status != 0)
            {
                break;
            }
        }
        else
        {
            WARN_MSG("Address 0x%x is not found in ade configuration structure", pSrc[i].address)
        }
    }
    return status;
}

static int32_t ReadConfigRegister(METIC_REGISTER *pSrc, int32_t numSrcRegisters,
                                  int32_t numDstRegisters, METIC_REGISTER *pDst)
{
    int32_t status = 0;
    int32_t index;
    int32_t i;
    // Iterates through list of registers in destination buffer (adeExampleNvmReg) and gets index of
    // the address present in source buffer (ADE_CONFIG_REG) and sends read command to Metrology IC
    // and updates the value in #adeExampleNvmReg.
    for (i = 0; i < numDstRegisters; i++)
    {
        index = GetRegIndex(pDst[i].address, numSrcRegisters, pSrc);
        if (index != -1)
        {
            status = ReadFromSlave(pSrc[index].address, &pSrc[index].value);
            memcpy(&pDst[i].value, &pSrc[index].value, 4);
            if (status != 0)
            {
                break;
            }
        }
        else
        {
            WARN_MSG("Address 0x%x is not found in ade configuration structure", pSrc[i].address)
        }
    }
    return status;
}

int32_t GetRegIndex(uint32_t address, int32_t numRegisters, METIC_REGISTER *pDst)
{
    int32_t index = -1;
    int32_t j;
    // Iterating through all the configuration registers in ADE9178, and finds the index of the
    // matching address.
    for (j = 0; j < numRegisters; j++)
    {
        if (address == pDst[j].address)
        {
            index = (int32_t)j;
            break;
        }
    }
    return index;
}

static void CopyNvmAddressToNvm(METIC_REGISTER *pSrc, int32_t numRegisters, int32_t *pDst)
{
    int32_t i;
    // Copies values into NVM_REG structure.
    for (i = 0; i < numRegisters; i++)
    {
        pDst[i] = pSrc[i].value;
    }
}

static void CopyNvmToNvmAddress(int32_t *pSrc, int32_t numRegisters, METIC_REGISTER *pDst)
{
    int32_t i;
    // Copies values from NVM_REG to #adeExampleNvmReg struct.
    for (i = 0; i < numRegisters; i++)
    {
        pDst[i].value = pSrc[i];
    }
}

static int32_t WriteToSlave(uint32_t address, int32_t *pValue)
{
    ADI_METIC_STATUS adeStatus = 0;
    int32_t status = 0;
    int32_t prevValue = *pValue;
    METIC_INSTANCE_INFO *pInfo;
    pInfo = GetAdeInstance();
    adeStatus = adi_metic_WriteRegister(pInfo->hAde, 0, (uint16_t)address, pValue);
    // If an error occurs, the value should point to the previous value.
    if (adeStatus != ADI_METIC_STATUS_SUCCESS)
    {
        DisplayErrorCode((uint16_t)address, 0, *pValue, adeStatus);
        memcpy(pValue, &prevValue, 4);
        status = -1;
    }
    return status;
}

static int32_t ReadFromSlave(uint32_t address, int32_t *pValue)
{
    ADI_METIC_STATUS adeStatus = 0;
    int32_t status = 0;
    int32_t prevValue = *pValue;
    METIC_INSTANCE_INFO *pInfo;
    pInfo = GetAdeInstance();
    adeStatus = adi_metic_ReadRegister(pInfo->hAde, 0, (uint16_t)address, 1, pValue);
    // If an error occurs, the value should point to the previous value.
    if (adeStatus != ADI_METIC_STATUS_SUCCESS)
    {
        DisplayErrorCode((uint16_t)address, 1, *pValue, adeStatus);
        memcpy(pValue, &prevValue, 4);
        status = -1;
    }
    return status;
}

/**
 * @}
 */
