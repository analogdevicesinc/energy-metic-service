/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file     dispatch_table.h
 * @brief    This file contains API declarations for dispatch table
 * @{
 */

#ifndef __DISPATCH_TABLE_H__
#define __DISPATCH_TABLE_H__

#include "cli_commands.h"
#include "cli_dispatch.h"
#include "cli_interface.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Command dispatch table
 * Add commands table here with function description
 */
static const Command dispatchTable[] = {
    {"start", "d", CmdStart, NOHIDE, "Starts calculating metrology parameters", "<num_cycles>",
     NULL, NULL},
    {"version", "s", CmdVersion, NOHIDE, "Gets Firmware version", "", NULL, NULL},
    {"setdisplay", "ss", CmdSetDisplay, NOHIDE, "Sets display of a parameter to on or off ",
     "<option>  <on|off>", NULL, DescDisplay},
    {"help", "s", CliHelp, NOHIDE, "command help", "<command>",
     "\t  Display command line help summary. command specific help is "
     "displayed  if argument <command> is "
     "given.\n\r",
     NULL},
#ifdef ENABLE_X86_BUILD
    {"close", "", CliClose, NOHIDE, "Closes all opened files", "", NULL, NULL},
    {"exit", "", CliExit, NOHIDE, "Exits", "", NULL, NULL},
#endif /* ENABLE_X86_BUILD */
#if BOARD_CFG_RESET_TYPE == 0
    {"reset", "s", CmdReset, NOHIDE, "H/W Reset of ADE9178 and ADCs", "", NULL, NULL},
#else
    {"reset", "ss", CmdReset, NOHIDE, "H/W Reset of Devices", "<device>",
     "\tDevice can be ade9178 or adc or all\n\r"
     "\tade9178 for hardware reset of ADE9178.\n\r"
     "\tadc for hardware reset of ADC.\n\r"
     "\tall for hardware reset of both ADE9178 and ADC.\n\r",
     NULL},
#endif
    {"manual", "s", CliCmdManual, NOHIDE, "Enables or disables manual mode", "<on|off>", NULL,
     NULL},
    {"setreg", "sss", CmdSetRegData, NOHIDE, "Writes ADE9178/ADC register",
     "<device> <register address> <register value>",
     "\tDevice can be ADE9178, ADC0, ADC1, ADC2, ADC3 or ALL_ADC\n\r"
     "\tAddress and value should be in hexadecimal e.g. setreg ADE9178 b 3289",
     NULL},
    {"getreg", "sss", CmdGetRegData, NOHIDE, "Reads ADE9178 register",
     "<device> <register address> <number of registers to read>",
     "\tDevice can be ADE9178, ADC0, ADC1, ADC2, ADC3 or ALL_ADC\n\r"
     "\tAddress should be in hexadecimal e.g. getreg ADE9178 b 1 \n\r"
     "\tnumber of register to read can be greater than one.\n\r",
     NULL},
    {"loadreg", "s", CmdLoadReg, NOHIDE, "Load registers value", NULL,
     "\tLoad registers value from flash to ADE9178\n\r", NULL},
    {"savereg", "s", CmdSaveReg, NOHIDE, "save registers value", NULL,
     "\tSave registers value from ADE9178 into flash\n\r", NULL},
    {"setconfig", "ss", CmdSetConfig, NOHIDE, "sets configurations",
     "<config choices> <config value>",
     "\tChoose config as shown below\r\n"
     "\t1 for voltage scale\r\n"
     "\t2 for current scale\r\n"
     "\t3 for aux scale\r\n",
     NULL},
    {"getconfig", "s", CmdGetConfig, NOHIDE, "gets configuration values", "<config choices>",
     "\tChoose config as shown below\r\n"
     "\t1 for voltage scale\r\n"
     "\t2 for current scale\r\n"
     "\t3 for aux scale\r\n",
     NULL},
    {"capturewfrm", "dd", CmdCaptureWfrm, NOHIDE,
     "Starts Waveform capture with a delay of configured cycles", "<wf src> <delay_num_cycles>",
     "\tChoose waveform source type to stream as shown below\r\n"
     "\t0 for ADC\r\n"
     "\t1 for PCF\r\n"
     "\tNumber of cycles to delay to start waveform capture\n\r",
     NULL},
    {"displaywfrm", "s", CmdDisplayWfrm, NOHIDE,
     "Displays waveform with 12 channels enabled for 100ms", "", NULL, NULL},
    {"getpulsetime", "d", CmdDisplayPulseTime, HIDE, "Displays timestamp of Events", "<src_id>",
     "\tChoose source ids to display pulse time\r\n"
     "\t0 for CF1\r\n"
     "\t1 for CF2\r\n",
     NULL},
    {"geterrorcount", "s", CmdDisplayErrorStatusCount, NOHIDE, "Gets errorstatus count occured", "",
     NULL, NULL}};

/**
 * @brief Get the number of commands in the dispatch table
 */
#define NUM_COMMANDS (sizeof(dispatchTable) / sizeof(Command))

#ifdef __cplusplus
}
#endif

#endif /* __DISPATCH_TABLE_H__ */

/**
 * @}
 */
