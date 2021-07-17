#ifndef CLIEN_MQTT_H
#define CLIEN_MQTT_H

#include <string.h>
#include <errno.h>
#include <stdio.h>

#include "usart.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "lwip/tcp.h"

#include "ip_addr.h"

#include "Node_config.h"

#include "MQTTPacket.h"

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
struct netconn *  cMqtt_Connect(int ip1, int ip2, int ip3, int ip4, u16_t port);

err_t cMqtt_Publish(struct netconn * conn, char * topic, char * payload, int paylondlen);

err_t cMqtt_Receive(struct netconn * conn, 	uint8_t *dataBuff, u16_t *dataBuffLen);

err_t cMqtt_Subscribe (struct netconn * conn, char * topic);

void cMqtt_Close(int sock);

#endif
