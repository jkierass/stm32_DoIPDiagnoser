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

#include <stdio.h>

#include "cm_ipc.h"

#ifndef IPC_CHECK_sbSEND_COMPLETED
/*
 Please make sure the below code is placed in FreeRTOSConfig.h:
   void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer, int is_receive);
   #define sbSEND_COMPLETED( pxStreamBuffer ) vGenerateRemoteInterrupt( pxStreamBuffer, 1 )
   #define sbSEND_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken ) vGenerateRemoteInterrupt( pxStreamBuffer, 1 )
   #define sbRECEIVE_COMPLETED( pxStreamBuffer ) vGenerateRemoteInterrupt( pxStreamBuffer, 0 )
   #define sbRECEIVE_COMPLETED_FROM_ISR( pxStreamBuffer, pxHigherPriorityTaskWoken )  vGenerateRemoteInterrupt( pxStreamBuffer, 0 )
   #define IPC_CHECK_sbSEND_COMPLETED
 */
#error "Please make sure the above comment instructions are followed"
#endif

#define HSEM_PROCESS		(29U)

#ifdef CORE_CM7
#define HSEM_IRQn               HSEM1_IRQn
#define HSEM_TX_ID              (30U) /* use HW semaphore 8*/
#define HSEM_RX_ID              (31U) /* use HW semaphore 9*/
#elif CORE_CM4
#define HSEM_IRQn               HSEM2_IRQn
#define HSEM_TX_ID              (31U) /* use HW semaphore 8*/
#define HSEM_RX_ID              (30U) /* use HW semaphore 9*/
#else
#error Invalid Cortex-M configuration
#endif

#if defined(__GNUC__)

/* TODO: add initialization (= 0) for CM7 ??? */
static volatile shared_ram_t shared_ram __attribute__((section(".shared_ram")));

#ifdef CORE_CM7
#define xrx_ctrl_buf	(shared_ram.cm4_to_cm7_handle)
#define xtx_ctrl_buf	(shared_ram.cm7_to_cm4_handle)

#define xrx_data_buf(index) (shared_ram.channels[index].cm4_to_cm7_handle)
#define xtx_data_buf(index) (shared_ram.channels[index].cm7_to_cm4_handle)
#elif CORE_CM4
#define xrx_ctrl_buf	(shared_ram.cm7_to_cm4_handle)
#define xtx_ctrl_buf	(shared_ram.cm4_to_cm7_handle)

#define xrx_data_buf(index) (shared_ram.channels[index].cm7_to_cm4_handle)
#define xtx_data_buf(index) (shared_ram.channels[index].cm4_to_cm7_handle)
#endif
#else
#error Only GCC is supported
#endif

void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer, int is_receive);
static void prvCoreInterruptHandler(int ctrl);

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
    prvCoreInterruptHandler(0);
  }
}

int ipc_init(void)
{
    /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();

#ifdef CORE_CM7
    shared_ram.cm7_to_cm4_handle = xMessageBufferCreateStatic(CM7_TO_CM4_CTRL_SIZE, (uint8_t*)shared_ram.cm7_to_cm4_buffer,(StaticMessageBuffer_t*) &shared_ram.cm7_to_cm4_xmsg);
    shared_ram.cm4_to_cm7_handle = xMessageBufferCreateStatic(CM4_TO_CM7_CTRL_SIZE, (uint8_t*)shared_ram.cm4_to_cm7_buffer,(StaticMessageBuffer_t*) &shared_ram.cm4_to_cm7_xmsg);

    for(int i = 0;i < IPC_NUMBER_OF_CHANNELS;++i){
        shared_ram.channels[i].cm7_to_cm4_handle = xMessageBufferCreateStatic(IPC_CHANNEL_BUFFER_SIZE, (uint8_t*)shared_ram.channels[i].cm7_to_cm4_buffer,(StaticMessageBuffer_t*) &shared_ram.channels[i].cm7_to_cm4_xmsg);
        shared_ram.channels[i].cm4_to_cm7_handle = xMessageBufferCreateStatic(IPC_CHANNEL_BUFFER_SIZE, (uint8_t*)shared_ram.channels[i].cm4_to_cm7_buffer,(StaticMessageBuffer_t*) &shared_ram.channels[i].cm4_to_cm7_xmsg);
    }
#endif

    return 0;
}

int ipc_start(void){
    /* Enable HSEM Interrupt */
    HAL_NVIC_SetPriority(HSEM_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(HSEM_IRQn);

    HAL_HSEM_ActivateNotification(__HAL_HSEM_SEMID_TO_MASK(HSEM_RX_ID));
    return 0;
}

static void prvCoreInterruptHandler(int ctrl)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    amp_ctrl_msg_t ctrl_msg;

    if (!xrx_ctrl_buf){
        return;
    }


	while(xMessageBufferReceiveFromISR(xrx_ctrl_buf, &ctrl_msg, sizeof(amp_ctrl_msg_t), &xHigherPriorityTaskWoken) == sizeof(amp_ctrl_msg_t)){
		if(ctrl_msg.is_receive){
			xMessageBufferSendCompletedFromISR(ctrl_msg.buffer,
											   &xHigherPriorityTaskWoken);
		}
		else {
			xMessageBufferReceiveCompletedFromISR(ctrl_msg.buffer,
											   &xHigherPriorityTaskWoken);
		}
	}
    /* Normal FreeRTOS yield from interrupt semantics, where
       xHigherPriorityTaskWoken is initialzed to pdFALSE and will then get set
       to pdTRUE if the interrupt safe API unblocks a task that has a priority
       above that of the currently executing task. */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void vGenerateRemoteInterrupt(void * xUpdatedMessageBuffer, int is_receive)
{
    MessageBufferHandle_t xUpdatedBuffer =
        (MessageBufferHandle_t)xUpdatedMessageBuffer;
    amp_ctrl_msg_t ctrl_msg;

	__DSB();
    if (xUpdatedBuffer != xtx_ctrl_buf && xUpdatedBuffer != xrx_ctrl_buf)
    {
		ctrl_msg.buffer = xUpdatedBuffer;
		ctrl_msg.is_receive = is_receive;
		if(xMessageBufferSend(xtx_ctrl_buf, &ctrl_msg, sizeof(amp_ctrl_msg_t), 0) != sizeof(amp_ctrl_msg_t)){
			/* Control message buffer overflow */
			Error_Handler();
		}

		/* Take the HW Semaphore with Process1 ID  */
		if(HAL_HSEM_Take(HSEM_TX_ID, HSEM_PROCESS) == HAL_OK)
		{
			/* Release the HW Semaphore */
			HAL_HSEM_Release(HSEM_TX_ID, HSEM_PROCESS);
		}
    }
}

size_t ipc_sendmsg(unsigned int channel, ipc_msg* msg, uint32_t size, int32_t timeout)
{
	size_t len;
    if(xtx_ctrl_buf == 0)
      return 0;
    if(channel >= IPC_NUMBER_OF_CHANNELS)
      return 0;

    if(timeout < 0)
    	len = xMessageBufferSend(xtx_data_buf(channel), msg, size, portMAX_DELAY);
    else
    	len = xMessageBufferSend(xtx_data_buf(channel), msg, size, (TickType_t)timeout);

    return len;
}

size_t ipc_recvmsg(unsigned int channel, ipc_msg* msg, uint32_t size, int32_t timeout)
{
    size_t len;

    if(channel >= IPC_NUMBER_OF_CHANNELS)
      return 0;

    if (timeout < 0)
        len = xMessageBufferReceive(xrx_data_buf(channel), msg, size, portMAX_DELAY);
    else
        len = xMessageBufferReceive(xrx_data_buf(channel), msg, size, (TickType_t)timeout);

    return len;
}
