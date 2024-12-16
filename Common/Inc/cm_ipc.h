/**
  ******************************************************************************
  * @file    cm_ipc.h
  * @author  MCD Application Team
  * @brief   Header for cm_ipc.h file
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

#ifndef __CMx_IPC_H
#define __CMx_IPC_H

#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void ipc_msg;

int ipc_init(void);
int ipc_start(void);
void ipc_isr(void);

size_t ipc_sendmsg(ipc_msg* msg, uint32_t size, int32_t timeout);
size_t ipc_recvmsg(ipc_msg* msg, uint32_t size, int32_t timeout);

#define CM7_TO_CM4_BUFFER_SIZE		(28*50)
#define CM4_TO_CM7_BUFFER_SIZE		(28*50)

typedef struct {
	MessageBufferHandle_t cm7_to_cm4_handle;
	MessageBufferHandle_t cm4_to_cm7_handle;
	StaticMessageBuffer_t cm7_to_cm4_xmsg;
	StaticMessageBuffer_t cm4_to_cm7_xmsg;
	uint32_t cm7_to_cm4_buffer[CM7_TO_CM4_BUFFER_SIZE/4];
	uint32_t cm4_to_cm7_buffer[CM4_TO_CM7_BUFFER_SIZE/4];
}shared_ram_t; // Ensure proper alignment;

#define HSEM_INIT_CM7 26U  // Semaphore for CM7 initialization
#define HSEM_INIT_CM4 27U  // Semaphore for CM4 initialization


#ifdef __cplusplus
}
#endif

#endif
