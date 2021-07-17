#ifndef DEBUG_INT_H
#define DEBUG_INT_H

#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "usart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "gpio.h"

typedef struct debugMsg{
	char * msg;
	int msgSize;
	int RLED;
	int GLED;
	int BLED;
}debugMsg;

void debugINT_start(osMessageQueueId_t debugMsgQueueHandle);

osStatus_t  debugInt_Send(osMessageQueueId_t debugMsgQueueHandle, char * msg, int msgLen, int RLED, int GLED, int BLED);

#endif
