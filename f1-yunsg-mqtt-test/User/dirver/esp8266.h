#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "main.h"

#define ESP8266_RX_BUF_SIZE         128
#define ESP8266_TX_BUF_SIZE         64

#define ESP8266_EOK                 0
#define ESP8266_ERROR               1
#define ESP8266_ETIMEOUT            2
#define ESP8266_EINVAL              3

#define ESP8266_STA_MODE            1
#define ESP8266_AP_MODE             2
#define ESP8266_STA_AP_MODE         3

#define ESP8266_SINGLE_CONNECTION   0
#define ESP8266_MULTI_CONNECTION    1

#define WIFI_SSID                   "aaa"
#define WIFI_PWD                    "18568601031"

#define TCP_SERVER_IP               "115.236.153.172"
#define TCP_SERVER_PORT             "32886"



void esp8266_init(void);
void esp8266_send_data(char *fmt, ...);
uint16_t esp8266_receive_data(char *recv_data);
uint8_t esp8266_send_command(char *cmd, char *res);

#endif


