/**
  ******************************************************************************
  * @file    cm_ipc.c
  * @author  MCD Application Team
  * @brief   Cortex-M IPC implementation module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include "stm32h7xx_hal.h"

#include "string.h"
#include <stdio.h>

#include "cm_ipc.h"

#ifndef IPC_CHECK_sbSEND_COMPLETED
/*
 Please make sure the below code is placed in FreeRTOSConfig.h:
   void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer);
   #define sbSEND_COMPLETED( pxStreamBuffer ) vGenerateRemoteInterrupt( pxStreamBuffer )
   #define sbSEND_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) vGenerateRemoteInterrupt( pxStreamBuffer )
   #define sbRECEIVE_COMPLETED( pxStreamBuffer ) vGenerateRemoteInterrupt( pxStreamBuffer )
   #define sbRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) vGenerateRemoteInterrupt( pxStreamBuffer )
 */
#error "Please make sure the above comment instructions are followed"
#endif

#define HSEM_PROCESS		(27U)

#ifdef CORE_CM7
#define HSEM_IRQn               HSEM1_IRQn
#define HSEM_TX_ID              (28U) /* use HW semaphore 8*/
#define HSEM_RX_ID              (29U) /* use HW semaphore 9*/

#define HSEM_TX_WAKEUP			(30U)
#define HSEM_RX_WAKEUP			(31U)
#elif CORE_CM4
#define HSEM_IRQn               HSEM2_IRQn
#define HSEM_TX_ID              (29U) /* use HW semaphore 8*/
#define HSEM_RX_ID              (28U) /* use HW semaphore 9*/

#define HSEM_TX_WAKEUP			(31U)
#define HSEM_RX_WAKEUP			(30U)
#else
#error Invalid Cortex-M configuration
#endif

#if defined(__GNUC__)

/* TODO: add initialization (= 0) for CM7 ??? */
static volatile shared_ram_t shared_ram __attribute__((section(".shared_ram")));


#ifdef CORE_CM7
#define xrx_data_buf	(shared_ram.cm4_to_cm7_handle)
#define xtx_data_buf	(shared_ram.cm7_to_cm4_handle)
#elif CORE_CM4
#define xrx_data_buf	(shared_ram.cm7_to_cm4_handle)
#define xtx_data_buf	(shared_ram.cm4_to_cm7_handle)
#endif
#else
#error Only GCC is supported
#endif

void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer);
static void prvCoreInterruptHandler(void);
static void prvCoreWakeupInterruptHandler(void);

/**
  * @brief Semaphore Released Callback.
  * @param SemMask: Mask of Released semaphores
  * @retval None
  */
void HAL_HSEM_FreeCallback(uint32_t SemMask)
{
  if((SemMask &  __HAL_HSEM_SEMID_TO_MASK(HSEM_RX_ID))!= 0)
  {
    /* Re-Activate HSEM notification */
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_RX_ID));
    /* New message received */
    prvCoreInterruptHandler();
  }

  if((SemMask & __HAL_HSEM_SEMID_TO_MASK(HSEM_TX_WAKEUP)) !=0){
	/* Re-Activate HSEM notification */
	HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_TX_WAKEUP));
	/* TX queue is empty => wake-up task trying to send a message */
	prvCoreWakeupInterruptHandler();
  }
}

int ipc_init(void)
{
    /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
#ifdef CORE_CM4
  	HAL_HSEM_Take(HSEM_INIT_CM4, 0);
#elif defined(CORE_CM7)
	memset((void*)(&shared_ram.cm7_to_cm4_buffer), 0, CM7_TO_CM4_BUFFER_SIZE);
	memset((void*)(&shared_ram.cm4_to_cm7_buffer), 0, CM4_TO_CM7_BUFFER_SIZE);
#endif
    shared_ram.cm7_to_cm4_handle = xMessageBufferCreateStatic(CM7_TO_CM4_BUFFER_SIZE, (uint8_t*)shared_ram.cm7_to_cm4_buffer,(StaticMessageBuffer_t*) &shared_ram.cm7_to_cm4_xmsg);
    shared_ram.cm4_to_cm7_handle = xMessageBufferCreateStatic(CM4_TO_CM7_BUFFER_SIZE, (uint8_t*)shared_ram.cm4_to_cm7_buffer,(StaticMessageBuffer_t*) &shared_ram.cm4_to_cm7_xmsg);

#ifdef CORE_CM7
    // Signal CM4 that CM7 has completed initialization
    HAL_HSEM_Release(HSEM_INIT_CM4, 0);
    // Wait for CM4 to complete initialization
    HAL_HSEM_Take(HSEM_INIT_CM7, 0);
#elif defined(CORE_CM4)
    // Signal CM7 that CM4 has completed initialization
    HAL_HSEM_Release(HSEM_INIT_CM7, 0);
#endif



    return 0;
}

int ipc_start(void){
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_RX_ID));
    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_TX_WAKEUP));
    return 0;
}

static void prvCoreInterruptHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (!xrx_data_buf)
    {
    	return;
    }


	xMessageBufferSendCompletedFromISR(xrx_data_buf,
									   &xHigherPriorityTaskWoken);

    /* Normal FreeRTOS yield from interrupt semantics, where
       xHigherPriorityTaskWoken is initialzed to pdFALSE and will then get set
       to pdTRUE if the interrupt safe API unblocks a task that has a priority
       above that of the currently executing task. */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void prvCoreWakeupInterruptHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (!xtx_data_buf)
    {
    	return;
    }


	xMessageBufferReceiveCompletedFromISR(xtx_data_buf,
									   &xHigherPriorityTaskWoken);

    /* Normal FreeRTOS yield from interrupt semantics, where
       xHigherPriorityTaskWoken is initialzed to pdFALSE and will then get set
       to pdTRUE if the interrupt safe API unblocks a task that has a priority
       above that of the currently executing task. */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer)
{
    MessageBufferHandle_t xUpdatedBuffer =
        (MessageBufferHandle_t)xUpdatedMessageBuffer;

	__DSB();
    if (xUpdatedBuffer == xtx_data_buf)
    {

        /* Take the HW Semaphore with Process1 ID  */
        if(HAL_HSEM_Take(HSEM_TX_ID, HSEM_PROCESS) == HAL_OK)
        {
            /* Release the HW Semaphore */
            HAL_HSEM_Release(HSEM_TX_ID, HSEM_PROCESS);
        }
    }
    else if(xUpdatedBuffer == xrx_data_buf){
    	/* Take the HW Semaphore with Process1 ID  */
		if(HAL_HSEM_Take(HSEM_RX_WAKEUP, HSEM_PROCESS) == HAL_OK)
		{
			/* Release the HW Semaphore */
			HAL_HSEM_Release(HSEM_RX_WAKEUP, HSEM_PROCESS);
		}
    }
}

size_t ipc_sendmsg(ipc_msg* msg, uint32_t size, int32_t timeout)
{
	size_t len;

    if(xtx_data_buf == 0)
      return 0;

    if(timeout < 0)
    	len = xMessageBufferSend(xtx_data_buf, msg, size, portMAX_DELAY);
    else
    	len = xMessageBufferSend(xtx_data_buf, msg, size, (TickType_t)timeout);

    return len;
}

size_t ipc_recvmsg(ipc_msg* msg, uint32_t size, int32_t timeout)
{
    size_t len;

    if (timeout < 0)
        len = xMessageBufferReceive(xrx_data_buf, msg, size, portMAX_DELAY);
    else
        len = xMessageBufferReceive(xrx_data_buf, msg, size, (TickType_t)timeout);

    return len;
}
