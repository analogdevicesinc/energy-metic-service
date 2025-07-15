/******************************************************************************
 Copyright (c) 2025  Analog Devices Inc.
******************************************************************************/

/*============= I N C L U D E S =============*/

#include "adi_evb.h"
#include "adi_metic.h"
#include "metic_hal.h"
#include "metic_service_interface.h"
#include "mxc_delay.h"
#include <stddef.h>
#include <stdint.h>

/*=============  C O D E  =============*/

static ADI_METIC_HANDLE hAde;

void AdeInitHal(ADI_METIC_HANDLE hInstance)
{
    hAde = hInstance;
}

void AdeHandleHostRdyErrCallback(uint32_t flag)
{
    uint8_t pinState;
    if ((flag & BOARD_CFG_HOST_RDY_PIN) != 0)
    {
        pinState = EvbGetPinState((uint32_t)BOARD_CFG_ADECOMM_PORT, BOARD_CFG_HOST_RDY_PIN);
        adi_metic_HostRdyCallback(hAde, pinState);
    }

    if ((flag & BOARD_CFG_HOST_ERR_PIN) != 0)
    {
        pinState = EvbGetPinState((uint32_t)BOARD_CFG_ADEIRQ_PORT, BOARD_CFG_HOST_ERR_PIN);
        adi_metic_HostErrCallback(hAde, pinState);
    }
}
