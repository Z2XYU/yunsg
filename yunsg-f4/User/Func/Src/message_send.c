#include "message_send.h"
#include "cmsis_os.h"
#include "user_TaskInit.h"
#include "usart.h"
#include "cJSON.h"
#include "string.h"
#include "stdlib.h"

/**
 * @brief 将一条语音消息加入发送队列
 * @param json 要发送的语音消息结构体
 */
void add_message_to_send_queue(VoiceJson_t *json)
{
    osMessageQueuePut(MQTTMessageSendQueueHandle, json, 0, 0);
}

/**
 * @brief 使用DMA通过UART发送VoiceJson_t结构体。
 *
 * 此函数通过UART2使用DMA方式发送提供的VoiceJson_t结构体，
 * 实现高效的非阻塞通信。
 *
 * @param json 要发送的VoiceJson_t结构体。
 */
void message_send(char *json_str)
{
    if (json_str != NULL)
    {
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)json_str, strlen(json_str));
    }
}

char *message_package(VoiceJson_t *json)
{

    cJSON *root = cJSON_CreateObject();

    // 添加字段
    cJSON_AddStringToObject(root, "topic", json->topic);

    cJSON *msg = cJSON_CreateObject();
    cJSON_AddStringToObject(msg, "option", json->msg.option);
    cJSON_AddStringToObject(msg, "value", json->msg.value);

    cJSON_AddItemToObject(root, "msg", msg);
    // 转为字符串
    char *json_str = cJSON_PrintUnformatted(root);

    
    strcat(json_str, "#");

    cJSON_Delete(msg);
    cJSON_Delete(root);


    return json_str;
}