#include "tasks.h"
#include "esp8266.h"
#include "cmsis_os.h"
#include "cJSON.h"
#include "stdio.h"
#include "string.h"
#include "mqtt.h"

extern uint8_t esp8266_rx_buf[ESP8266_RX_BUF_SIZE];

/**
 * @brief 解析 JSON 格式字符串，提取 topic 和 message 字段并打印
 * @param json_str: 输入的 JSON 字符串
 */

void parse_json_msg(const char *json_str, mqttMsg *msg)
{
    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL)
    {
        printf("JSON parse error!\r\n");
        return;
    }

    cJSON *action = cJSON_GetObjectItem(root, "action");
    cJSON *option = cJSON_GetObjectItem(root, "option");
    cJSON *location = cJSON_GetObjectItem(root, "cabinetLocation");

    if (cJSON_IsString(action) && action->valuestring)
    {
        strncpy(msg->action, action->valuestring, sizeof(msg->action) - 1);
        msg->action[sizeof(msg->action) - 1] = '\0';
        printf("action: %s\r\n", msg->action);
    }
    else
    {
        msg->action[0] = '\0';
    }

    if (cJSON_IsString(option) && option->valuestring)
    {
        strncpy(msg->option, option->valuestring, sizeof(msg->option) - 1);
        msg->option[sizeof(msg->option) - 1] = '\0';
        printf("option: %s\r\n", msg->option);
    }
    else
    {
        msg->option[0] = '\0';
    }

    if (cJSON_IsNumber(location))
    {
        msg->cabinetLocation = location->valueint;
        printf("cabinetLocation: %d\r\n", msg->cabinetLocation);
    }
    else
    {
        msg->cabinetLocation = -1;
        printf("cabinetLocation 字段缺失或不是数字\r\n");
    }

    cJSON_Delete(root);
}

void msg_proc(mqttMsg *msg)
{
    printf("msg.l:%d\n",msg->cabinetLocation);
    switch (msg->cabinetLocation)
    {
    case 1:
        if (strcmp(msg->option, "open") == 0)
        {
            printf("No.1 open\n");
        }
        else if (strcmp(msg->option, "close") == 0)
        {
            printf("No.1 close\n");
        }
        break;

    case 2:
        if (strcmp(msg->option, "open") == 0)
        {
            printf("No.2 open\n");
        }
        else if (strcmp(msg->option, "close") == 0)
        {
            printf("No.2 close\n");
        }
        break;

    case 3:
        if (strcmp(msg->option, "open") == 0)
        {
            printf("No.3 open\n");
        }
        else if (strcmp(msg->option, "close") == 0)
        {
            printf("No.3 close\n");
        }
        break;

    case 4:
        if (strcmp(msg->option, "open") == 0)
        {
            printf("No.4 open\n");
        }
        else if (strcmp(msg->option, "close") == 0)
        {
            printf("No.4 close\n");
        }
        break;

    case 5:
        if (strcmp(msg->option, "open") == 0)
        {
            printf("No.5 open\n");
        }
        else if (strcmp(msg->option, "close") == 0)
        {
            printf("No.5 close\n");
        }
        break;

    default:
        printf("error\n");
        break;
    }
}

void control_task(void *argument)
{

    esp8266_init();
    mqtt_init();

    //osMessageQueueReset(mqttQueueHandle);

    while (1)
    {
        char recv_msg[128] = {0};
        mqttMsg mqtt_packet;
        int16_t len = esp8266_receive_data(recv_msg);
        if (len > 0)
        {
            printf("recvMsg: %s\n", recv_msg);

            char *json_start = strchr(recv_msg, '{');
            if (json_start)
            {
                parse_json_msg(json_start, &mqtt_packet);
                msg_proc(&mqtt_packet);
            }
            else
            {
                printf("未找到JSON起始符\n");
            }
        }

        osDelay(100);
    }
}