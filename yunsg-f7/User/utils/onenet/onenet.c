#include "onenet.h"
#include "onenet_config.h"
#include "string.h"
#include "4g_d43.h"
#include "dwt_delay.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "usart.h"

uint8_t onenet_connect(void)
{
    d43_send_at("AT+CGATT=1");  //注册网络

    if(!d43_wait(3000)) return 0;

    d43_send_at("AT+QMTOPEN=0,\"183.230.40.39\",6002"); //打开MQTT连接
    osDelay(2000);

    char cmd[128];
    sprintf(cmd,"AT+QMTCONN=0,\"%s\",\"%s\",\"\"",DEVICE_ID,API_KEY);

    d43_send_at(cmd);
    return d43_wait(3000);
}

uint8_t onenet_publish(const char* data)
{
    char cmd[64];
    sprintf(cmd,"AT+QMTPUB=0,0,0,0,\"%s\"",TOPIC);
    d43_send_at(cmd);

    osDelay(200);

    HAL_UART_Transmit(&huart3,(uint8_t*)data,strlen(data),1000);
    HAL_UART_Transmit(&huart3,(uint8_t*)"\x1A",1,100);   //结束符

    return d43_wait(2000);
}


