/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/*============= I N C L U D E S =============*/

#include "adi_cli.h"
#include "adi_evb.h"
#include "message.h"
#include "metic_example.h"
#include "status.h"

/**
 * Entry point
 */

int main()
{
    int32_t status = 0;
    /* Initialize board */
    status = InitBoard();
    /* Initialize services needed for the example*/
    if (status == 0)
    {
        status = InitServices();
    }
    if (status == 0)
    {
        status = RunExample();
    }

    return status;
}

#ifndef ENABLE_RTOS_BUILD
int32_t RunExample()
{
    int32_t criticalError = 0;
    int32_t status = SYS_STATUS_SUCCESS;
    status = InitExampleStates();
    while (criticalError == 0)
    {
        status = HandleRunState();
        criticalError = HandleError(status);
        if (adi_cli_FlushMessages() == 0)
        {
            if (status != SYS_STATUS_RUNNING)
            {
                adi_cli_Interface();
            }
        }
    }
    return status;
}
#endif
