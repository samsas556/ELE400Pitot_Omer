#include "Client_MQTT.h"

/* Function cMqtt_Connect
 *
 * DESC 	: Connets to an MQTT broker
 *
 * IN		: ip[1..4] 	=> xxx.xxx.xxx.xxx adress of destination
 * 			  port		=> port number to conenct to
 *
 * Return 	: Created netconn pointer
 *
 * Example of usage :
 *	struct netconn * conn;
 *	conn  = cMqtt_Connect();
 */
struct netconn *  cMqtt_Connect(int ip1, int ip2, int ip3, int ip4, u16_t port){
	// Variables
	struct netconn * conn;
	ip_addr_t addr;
	IP4_ADDR(&addr,CONF_IP1,CONF_IP2,CONF_IP3,CONF_IP4);
	err_t connectStatus = ERR_OK;
	uint8_t uartSendBuf[50] = {0};
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	char buf[200];
	int buflen = sizeof(buf);
	err_t sendStatus = ERR_OK;
	int packetLen = 0;

	//// Establish connection with broker
	// create a netconn
	conn = netconn_new(NETCONN_TCP);

	// connect to broker
	if (conn != NULL){
		connectStatus = netconn_connect(conn, &addr, CONF_PORT);
		while(connectStatus != ERR_OK){
			// failed to connect try again
			snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nERROR : Return status of connect : %d\r\n", connectStatus);
			HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
			connectStatus = netconn_connect(conn, &addr, CONF_PORT);

			osDelay(50);
		}

		snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nSUCCESS : Return status of connect : %d\r\n", connectStatus);
		HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	}

	//// Connect to the broker
	// create connection parameters
	data.clientID.cstring = CONF_CLIENT_ID;
	data.keepAliveInterval = CONF_KEEPALIVE_INTERVAL;
	data.cleansession = 1;
	data.username.cstring = CONF_USERNAME;
	data.password.cstring = CONF_PASSWORD;
	data.MQTTVersion = 4;
	// Serialize packet
	packetLen = MQTTSerialize_connect((unsigned char *)buf, buflen, &data);
	// Send packet
	sendStatus = netconn_write(conn, buf, packetLen, NETCONN_COPY);
	snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nSENDED connect : status mqtt connect : %d\r\n", sendStatus);
	HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	return conn;
}

err_t cMqtt_Publish(struct netconn * conn, char * topic, char * payload, int payloadlen){
	// variables
	err_t sendStatus = ERR_OK;
	uint8_t uartSendBuf[50] = {0};
	MQTTString topicString = MQTTString_initializer;
	int messageLen = 0;
	// TODO algo size
	char buf[payloadlen + strlen(topic) + 10];
	int buflen = sizeof(buf);

	// Construct publish packet
	topicString.cstring = topic;
	messageLen = MQTTSerialize_publish((unsigned char *)buf,  buflen, 0, 0, 0, 0, topicString, (unsigned char *)payload, payloadlen);

	// send data
	sendStatus = netconn_write(conn, buf, messageLen, NETCONN_COPY );
	if(sendStatus != ERR_OK){
	  snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nReturn status of send : %d\r\n", sendStatus);
	  HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	}else{
		snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nSended message with SUCCESS, len of meg : %d\r\n", messageLen);
		HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	}

	return sendStatus;
}

err_t cMqtt_Receive(struct netconn * conn, 	uint8_t *dataBuff, u16_t *dataBuffLen){
	int receiveStatus = 0;
	uint8_t uartSendBuf[50] = {0};
	struct netbuf  *buf;

	// test if there is something to receive
	if(osMessageQueueGetCount(conn->recvmbox) == 0){
		snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nNothing to receive\r\n");
		HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
		return ERR_OK;
	}

	// receive something
	//memset(recvBuf,0,size);

	receiveStatus = netconn_recv(conn, &buf);
	if(receiveStatus != ERR_OK){
		  snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nReturn status of send : %d\r\n", receiveStatus);
		  HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	}else{
		  netbuf_data(buf, (void**) &dataBuff, dataBuffLen);
		  snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nreceived Data : %s\r\n", dataBuff);
		  HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	}



	// emptying buffer
	while (netbuf_next(buf) >= 0);
	netbuf_delete(buf);

	return receiveStatus;
}

err_t cMqtt_Subscribe (struct netconn * conn, char * topic){
	// variables
	err_t subStatus = ERR_OK;
	uint8_t uartSendBuf[50] = {0};
	MQTTString topicString = MQTTString_initializer;
	int messageLen = 0;
	// TODO algo size
	char buf[200];
	int buflen = sizeof(buf);
	int req_qos = 0;

	// Construct publish packet
	topicString.cstring = topic;
	messageLen = MQTTSerialize_subscribe((unsigned char *)buf, buflen, 0, 0, 1, &topicString, &req_qos);

	// send data
	subStatus = netconn_write(conn, buf, messageLen, NETCONN_COPY );
	if(subStatus != ERR_OK){
	  snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\nReturn status of Subscribe : %d\r\n", subStatus);
	  HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	}else{
		snprintf((char *)uartSendBuf,sizeof(uartSendBuf),"\r\Subscribed with SUCCESS, len of msg : %d\r\n", messageLen);
		HAL_UART_Transmit(&huart3,uartSendBuf,strlen((char *)uartSendBuf),10);
	}

	return subStatus;
};

void client_mqttClose(int sock){
	close(sock);
}



