#include "debug_interface.h"

void debugINT_start(osMessageQueueId_t debugMsgQueueHandle){
	// Variables
	debugMsg recvMsg;
	osStatus_t status;
	uint8_t uartSendBuf[50] = {0};

	while(1){
		status = osMessageQueueGet(debugMsgQueueHandle, &recvMsg, NULL, osWaitForever);
		if(status == osOK){
			// message received
			// Send uart message
			if(recvMsg.msg != NULL){
				HAL_UART_Transmit(&huart3,(uint8_t *)recvMsg.msg,strlen((char *)recvMsg.msg),10);
			}

			// Change LED state
			// Green
			if (recvMsg.GLED >= 0){
				if(recvMsg.GLED > 0){
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
				}
			}
			// Blue
			if (recvMsg.BLED >= 0){
				if(recvMsg.BLED > 0){
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
				}
			}
			// Red
			if (recvMsg.RLED >= 0){
				if(recvMsg.RLED > 0){
					HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, GPIO_PIN_RESET);
				}
			}
		}else{
			// Error
			snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nERROR : cannot receive debug message queue : %d\r\n", status);
			HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
		}
	}
}

osStatus_t  debugInt_Send(osMessageQueueId_t debugMsgQueueHandle, char * msg, int msgLen, int RLED, int GLED, int BLED){
	// Construct message to send
	debugMsg toSend;
	toSend.msg = msg;
	toSend.msgSize = msgLen;
	toSend.RLED = RLED;
	toSend.GLED = GLED;
	toSend.BLED = BLED;

	// Send the message
	return osMessageQueuePut(debugMsgQueueHandle, &toSend, 0U, 0);
}
