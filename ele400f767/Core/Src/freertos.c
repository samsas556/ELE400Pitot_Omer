/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "../Inc/usart.h"
#include "../Inc/i2c.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "lwip/tcp.h"

#include "ip_addr.h"

#include "Client_MQTT.h"

#include "debug_interface.h"

#include "pitot.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for mqtt_Client */
osThreadId_t mqtt_ClientHandle;
uint32_t mqtt_ClientBuffer[ 6144 ];
osStaticThreadDef_t mqtt_ClientControlBlock;
const osThreadAttr_t mqtt_Client_attributes = {
		.name = "mqtt_Client",
		.cb_mem = &mqtt_ClientControlBlock,
		.cb_size = sizeof(mqtt_ClientControlBlock),
		.stack_mem = &mqtt_ClientBuffer[0],
		.stack_size = sizeof(mqtt_ClientBuffer),
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for pitot_task */
osThreadId_t pitot_taskHandle;
uint32_t pitot_taskBuffer[ 1024 ];
osStaticThreadDef_t pitot_taskControlBlock;
const osThreadAttr_t pitot_task_attributes = {
		.name = "pitot_task",
		.cb_mem = &pitot_taskControlBlock,
		.cb_size = sizeof(pitot_taskControlBlock),
		.stack_mem = &pitot_taskBuffer[0],
		.stack_size = sizeof(pitot_taskBuffer),
		.priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for debugMsgQueue */
osMessageQueueId_t debugMsgQueueHandle;
uint8_t debugMsgQueueBuffer[ 4 * sizeof( debugMsg ) ];
osStaticMessageQDef_t debugMsgQueueControlBlock;
const osMessageQueueAttr_t debugMsgQueue_attributes = {
		.name = "debugMsgQueue",
		.cb_mem = &debugMsgQueueControlBlock,
		.cb_size = sizeof(debugMsgQueueControlBlock),
		.mq_mem = &debugMsgQueueBuffer,
		.mq_size = sizeof(debugMsgQueueBuffer)
};
/* Definitions for pitot_queue */
osMessageQueueId_t pitot_queueHandle;
uint8_t pitot_queueBuffer[ 8 * sizeof( double ) ];
osStaticMessageQDef_t pitot_queueControlBlock;
const osMessageQueueAttr_t pitot_queue_attributes = {
		.name = "pitot_queue",
		.cb_mem = &pitot_queueControlBlock,
		.cb_size = sizeof(pitot_queueControlBlock),
		.mq_mem = &pitot_queueBuffer,
		.mq_size = sizeof(pitot_queueBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_mqtt_Client(void *argument);
void Start_pitot_task(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* creation of debugMsgQueue */
	debugMsgQueueHandle = osMessageQueueNew (4, sizeof(debugMsg), &debugMsgQueue_attributes);

	/* creation of pitot_queue */
	pitot_queueHandle = osMessageQueueNew (8, sizeof(double), &pitot_queue_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of mqtt_Client */
	mqtt_ClientHandle = osThreadNew(Start_mqtt_Client, NULL, &mqtt_Client_attributes);

	/* creation of pitot_task */
	pitot_taskHandle = osThreadNew(Start_pitot_task, NULL, &pitot_task_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Start_mqtt_Client */
/**
 * @brief  Function implementing the mqtt_Client thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_mqtt_Client */
void Start_mqtt_Client(void *argument)
{
	/* init code for LWIP */
	MX_LWIP_Init();
	/* USER CODE BEGIN Start_mqtt_Client */

	//struct netconn * conn;d

	// Connect to server
	//conn  = cMqtt_Connect(10,214,96,60,1883);
	int compteur=0;
	double pressionMoy=0;

	/* Infinite loop */
	for(;;)
	{

		double pression;

		osStatus_t status_queue = osMessageQueueGet(pitot_queueHandle,&pression,0,0);
		if(status_queue == osOK){
			char string[40];

			pressionMoy+=pression;
			//cMqtt_Publish(conn, "1/data/1", string, strlen(string));
			compteur ++;
			if(compteur>=10){
				sprintf(string, "vitesse(m/s) %.4lf \n\r", pressionMoy/10);
				HAL_UART_Transmit(&huart3, (uint8_t*)string, strlen(string), HAL_MAX_DELAY);
				compteur=0;
				pressionMoy=0;
			}

		}

		osDelay(100);
	}
	/* USER CODE END Start_mqtt_Client */
}

/* USER CODE BEGIN Header_Start_pitot_task */
/**
 * @brief Function implementing the pitot_task thread.d
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_pitot_task */
void Start_pitot_task(void *argument)
{
	/* USER CODE BEGIN Start_pitot_task */
	/* Infinite loop */

	double speed;
	for(;;)
	{

		HAL_StatusTypeDef ret = !HAL_OK;
		while(ret != HAL_OK){

			ret = init_pitot(&hi2c2,START_AVERAGE16);

		}

		HAL_GPIO_TogglePin (Led2_GPIO_Port, Led2_Pin);
		osDelay(100);

		if(read_pitot_speed(&hi2c2,&speed))
			osMessageQueuePut(pitot_queueHandle,&speed,0,0);
		/* USER CODE END WHILE */
	}
	/* USER CODE END Start_pitot_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
