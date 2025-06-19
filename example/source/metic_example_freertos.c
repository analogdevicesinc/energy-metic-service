/******************************************************************************
 Copyright (c) 2023 - 2025  Analog Devices Inc.
******************************************************************************/

/**
 * @file        metic_example_freertos.c
 * @brief        * This file contains the definitions and implementations of the command handling
 thread
 * and the metrology output processing thread. These threads are responsible for managing
 * command execution and processing metrology data in a FreeRTOS environment.

 * @{
 */

#include "ade_example.h"
#include "app_cfg.h"
#include "message.h"
#include "osal_wait.h"
#include "semphr.h"
#include "status.h"

/** Flag to notify met output thread */
#define MET_SERVICE_RMSONERDY_FLAG 0x01

static void MetOutputThread(void *pArg);
static void CmdThread(void *pArg);

static SemaphoreHandle_t xBinarySemaphore;
static SemaphoreHandle_t xReadWriteMutex;

int32_t InitThreads(void)
{
    int32_t status = SYS_STATUS_SUCCESS;
    BaseType_t threadStatus = pdPASS;
    ADE_EXAMPLE *pExample = GetAdeExample();
    threadStatus = xTaskCreate(CmdThread, "CmdThread", APP_CFG_THREAD_STACK_SIZE, NULL,
                               APP_CFG_CMD_THREAD_PRIORITY, &pExample->threadInfo.cmdThread);
    if (threadStatus == pdPASS)
    {
        threadStatus =
            xTaskCreate(MetOutputThread, "MetOutputThread", APP_CFG_THREAD_STACK_SIZE, NULL,
                        APP_CFG_MET_OUTPUT_THREAD_PRIORITY, &pExample->threadInfo.metOutputThread);
    }
    if (threadStatus == pdPASS)
    {
        xBinarySemaphore = xSemaphoreCreateBinary();
        xReadWriteMutex = xSemaphoreCreateMutex();
    }
    else
    {
        status = -1;
    }
    return status;
}

void MetOutputThread(void *pArg)
{
    int32_t criticalError = 0;
    ADE_EXAMPLE *pExample = GetAdeExample();
    if (pArg != NULL)
    {
        // Added to resolve the warning
    }
    while (criticalError == 0)
    {
        xTaskNotifyWait(0, MET_SERVICE_RMSONERDY_FLAG, NULL, portMAX_DELAY);
        if (pExample->state == SYS_STATUS_RUNNING)
        {
            CollectMetOutputs();
        }
    }
}

void CmdThread(void *pArg)
{
    int32_t status = 0;
    int32_t criticalError = 0;
    if (pArg != NULL)
    {
        // Added to resolve the warning
    }
    status = InitExampleStates();
    while (criticalError == 0)
    {
        status = HandleRunState();
        criticalError = HandleError(status);
        if (EvbFlushMessages() == 0)
        {
            if (status != SYS_STATUS_RUNNING)
            {
                adi_cli_Interface();
            }
        }
    }
}

int32_t OsalSuspend(void *hUser, volatile uint8_t *pSuspendState, uint32_t timeout)
{
    int32_t status = 0;
    // Added to resolve the warning
    *pSuspendState = 1;
    if (hUser != NULL)
    {
        if (xSemaphoreTake(xBinarySemaphore, timeout) == pdTRUE)
        {
            /** waiting here for xBinarySemaphore to be posted*/
        }
        else
        {
            status = -1;
        }
    }
    else
    {
        status = -1;
    }
    return status;
}

int32_t OsalResume(void *hUser, volatile uint8_t *pSuspendState)
{
    int32_t status = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // Added to resolve the warning
    *pSuspendState = 1;

    if (hUser != NULL)
    {
        if (xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken) == pdTRUE)
        {
            /** posting xBinarySemaphore */
        }
    }
    else
    {
        status = -1;
    }

    return status;
}

void NotifyMetOutputThread(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    ADE_EXAMPLE *pExample = GetAdeExample();
    xTaskNotifyFromISR(pExample->threadInfo.metOutputThread, MET_SERVICE_RMSONERDY_FLAG, eSetBits,
                       &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/**
 * @}
 */
