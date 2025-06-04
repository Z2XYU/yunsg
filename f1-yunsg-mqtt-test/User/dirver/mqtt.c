#include "mqtt.h"
#include "stdio.h"
#include "esp8266.h"
#include "delay.h"


const uint8_t parket_connetAck[] = {0x20,0x02,0x00,0x00};
const uint8_t parket_disconnet[] = {0xe0,0x00};
const uint8_t parket_heart[] = {0xc0,0x00};
const uint8_t parket_heart_reply[] = {0xc0,0x00};
const uint8_t parket_subAck[] = {0x90,0x03};

uint8_t mqtt_set_userinfo(char *client_id,char *client_username,char *client_password)
{
	char cmd[64];
	sprintf(cmd,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"/mqtt\"\r\n",client_id,client_username,client_password);
	return esp8266_send_command(cmd,"OK");
}

uint8_t mqtt_connect_server(char *mqtt_server_ip,char *mqtt_server_port)
{
	char cmd[64];
	sprintf(cmd,"AT+MQTTCONN=0,\"%s\",%s,0\r\n",mqtt_server_ip,mqtt_server_port);
	return esp8266_send_command(cmd,"OK");
}

uint8_t mqtt_subscribe_topic(void)
{
	char cmd[64];
	sprintf(cmd,"AT+MQTTSUB=0,\"control\",0\r\n");
	return esp8266_send_command(cmd,"OK");
}

uint8_t mqtt_publish_data(char *mqtt_topic,char* mqtt_message)
{
	char cmd[64];
	sprintf(cmd,"AT+MQTTPUB=0,\"%s\",\"%s\",1,0\r\n",mqtt_topic,mqtt_message);
	return esp8266_send_command(cmd,"OK");
}

void mqtt_send_data(uint8_t *buf,uint16_t len)
{
    esp8266_send_data((char *)buf, len);
}

void mqtt_send_heart(void)
{
    mqtt_send_data((uint8_t *)parket_heart,sizeof(parket_heart));
}

void mqtt_disconnect(void)
{
    mqtt_send_data((uint8_t *)parket_disconnet,sizeof(parket_disconnet));
}

void mqtt_reconnect(void)
{
	printf("6. 重新连接MQTT服务器 MQTT_SERVER_IP:%s MQTT_SERVER_PORT:%s\n",MQTT_SERVER_IP,MQTT_SERVER_PORT);
	while(mqtt_connect_server(MQTT_SERVER_IP,MQTT_SERVER_PORT))
	{
		//HAL_IWDG_Refresh(&hiwdg);
		mdelay(3000);
	}
}

void mqtt_init(void)
{
	printf("6. 配置MQTT用户信息 MQTT_CLIENT_ID:%s MQTT_CLIENT_USERNAME:%s MQTT_CLIENT_PASSWORD:%s\n",MQTT_CLIENT_ID,MQTT_CLIENT_USERNAME,MQTT_CLIENT_PASSWORD);
	while(mqtt_set_userinfo(MQTT_CLIENT_ID,MQTT_CLIENT_USERNAME,MQTT_CLIENT_PASSWORD))
	{
		mdelay(3000);
	}
	
	printf("7. 连接MQTT服务器 MQTT_SERVER_IP:%s MQTT_SERVER_PORT:%s\n",MQTT_SERVER_IP,MQTT_SERVER_PORT);
	// while(mqtt_connect_server(MQTT_SERVER_IP,MQTT_SERVER_PORT))
	// {
	// 	mdelay(3000);
	// }

	mqtt_connect_server(MQTT_SERVER_IP,MQTT_SERVER_PORT);

	printf("8. 订阅control话题...\r\n");
	// while (mqtt_subscribe_topic())
	// {
	// 	mdelay(3000);
	// }

	mqtt_subscribe_topic();

	
	printf("9. 测试通信...\r\n");
	char *topic="control";
	char *msg="caonima";
	mqtt_publish_data(topic,msg);
}

