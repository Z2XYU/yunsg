#include "main.h"
#include "tasks.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "usart.h"
#include "cJSON.h"
#include "string.h"

extern uint8_t wifi_rx_buffer[WIFI_RX_BUFFER_SIZE];
extern DMA_HandleTypeDef hdma_usart2_rx;

static wifiJson_t wifi_control_data_parse(wifiMessage_t msg)
{
    wifiJson_t wifi_data = {0};

    char json_str[WIFI_RX_BUFFER_SIZE + 1] = {0};
    memcpy(json_str, msg.data, msg.length);
    json_str[msg.length] = '\0';

    // 找到第一个'{'和最后一个'}'
    char *start = strchr(json_str, '{');
    char *end = strrchr(json_str, '}');

    if (start && end && end > start)
    {
        size_t json_len = end - start + 1;

        char json_data[WIFI_RX_BUFFER_SIZE] = {0};

        if (json_len < sizeof(json_data))
        {
            memcpy(json_data, start, json_len);
            json_data[json_len] = '\0';

            // 解析数据
            cJSON *root = cJSON_Parse(json_data);
            if (root == NULL)
            {
                printf("JSON解析失败\n");
                return wifi_data;
            }
            // 解析字段 话题
            cJSON *topic = cJSON_GetObjectItem(root, "topic");
            if (topic && cJSON_IsString(topic))
            {
                strncpy(wifi_data.topic, topic->valuestring, sizeof(wifi_data.topic) - 1);
                wifi_data.topic[sizeof(wifi_data.topic) - 1] = '\0';
            }
            // 解析消息
            cJSON *message = cJSON_GetObjectItem(root, "msg");
            if (message)
            {
                cJSON *action = cJSON_GetObjectItem(message, "action");
                if (action && cJSON_IsString(action))
                {
                    strncpy(wifi_data.msg.action, action->valuestring, sizeof(wifi_data.msg.action) - 1);
                    wifi_data.msg.action[sizeof(wifi_data.msg.action) - 1] = '\0';
                }

                cJSON *option = cJSON_GetObjectItem(message, "option");
                if (option && cJSON_IsString(option))
                {
                    strncpy(wifi_data.msg.option, option->valuestring, sizeof(wifi_data.msg.option) - 1);
                    wifi_data.msg.option[sizeof(wifi_data.msg.option) - 1] = '\0';
                }

                cJSON *cabinetLocation = cJSON_GetObjectItem(message, "cabinetLocation");
                if (cabinetLocation && cJSON_IsNumber(cabinetLocation))
                {
                    wifi_data.msg.cabinetLocation = cabinetLocation->valueint;
                }
            }
            cJSON_Delete(root);
        }
    }
    return wifi_data;
}

static option_e control_cmd(const char *cmd)
{
    if (strcmp(cmd, "open") == 0)
        return OPTION_CABINET_OPEN;
    else if (strcmp(cmd, "close") == 0)
        return OPTION_CABINET_CLOSE;
    else
        return OPTION_UNKNOWN;
}

static void control(wifiJson_t cmd)
{
    option_e option = control_cmd(cmd.msg.option);

    switch (option)
    {
    case OPTION_CABINET_OPEN:
        printf("柜门%d已打开\n", cmd.msg.cabinetLocation);
        break;
    case OPTION_CABINET_CLOSE:
        printf("柜门%d已关闭\n", cmd.msg.cabinetLocation);
        break;
    default:
        break;
    }
}

void wifiControlTask(void *argument)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, wifi_rx_buffer, sizeof(wifi_rx_buffer));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

    wifiMessage_t msg;
    while (1)
    {
        if (osMessageQueueGet(wifiMsgQueueHandle, &msg, NULL, osWaitForever) == osOK)
        {
            printf("已接受消息\n");
            wifiJson_t control_cmd = wifi_control_data_parse(msg);

            control(control_cmd);

            // HAL_UART_Transmit_DMA(&huart2, msg.data, msg.length);
        }
    }
}