#include "message_receive.h"
#include "string.h"
#include "cJSON.h"
#include "stdio.h"
#include "cmsis_os.h"
#include "user_TaskInit.h"

/**
 * @brief   解析MQTT消息，提取JSON有效载荷信息
 * @param   msg: MQTTMessage_t 类型的原始消息
 * @retval  MQTTJson_: 结构化的JSON数据，包括topic、ControlMessage_t 字段
 */
ControlJson_t mqtt_message_parse(MQTTMessage_t msg)
{
    ControlJson_t mqtt_json = {0};

    char json_str[MQTT_MESSAGE_BUFFER_SIZE + 1] = {0};
    memcpy(json_str, msg.data, msg.length);
    json_str[msg.length] = '\0';

    // 找到第一个{ 和最后一个 }
    char *start = strchr(json_str, '{');
    char *end = strrchr(json_str, '}');

    if (start && end && end > start)
    {
        size_t json_len = end - start + 1;

        char json_data[MQTT_MESSAGE_BUFFER_SIZE] = {0};

        if (json_len < sizeof(json_data))
        {
            memcpy(json_data, start, json_len);
            json_data[json_len] = '\0';

            // 解析数据
            cJSON *root = cJSON_Parse(json_data);
            if (root == NULL)
            {
                printf("JSON解析失败\n");
                return mqtt_json;
            }
            // 解析字段 话题
            cJSON *topic = cJSON_GetObjectItem(root, "topic");
            if (topic && cJSON_IsString(topic))
            {
                strncpy(mqtt_json.topic, topic->valuestring, sizeof(mqtt_json.topic) - 1);
                mqtt_json.topic[sizeof(mqtt_json.topic) - 1] = '\0';
            }
            // 解析消息
            cJSON *message = cJSON_GetObjectItem(root, "msg");
            if (message)
            {
                cJSON *action = cJSON_GetObjectItem(message, "action");
                if (action && cJSON_IsString(action))
                {
                    strncpy(mqtt_json.msg.action, action->valuestring, sizeof(mqtt_json.msg.action) - 1);
                    mqtt_json.msg.action[sizeof(mqtt_json.msg.action) - 1] = '\0';
                }

                cJSON *option = cJSON_GetObjectItem(message, "option");
                if (option && cJSON_IsString(option))
                {
                    strncpy(mqtt_json.msg.option, option->valuestring, sizeof(mqtt_json.msg.option) - 1);
                    mqtt_json.msg.option[sizeof(mqtt_json.msg.option) - 1] = '\0';
                }

                cJSON *cabinetLocation = cJSON_GetObjectItem(message, "cabinetLocation");
                if (cabinetLocation && cJSON_IsNumber(cabinetLocation))
                {
                    mqtt_json.msg.cabinetLocation = cabinetLocation->valueint;
                }
            }
            cJSON_Delete(root);
        }
    }
    return mqtt_json;
}


/**
 * @brief  解析控制命令字符串并转换为对应的枚举值
 * @param  cmd: 输入的命令字符串，如 "open" 或 "close"
 * @retval Option_e: 返回对应的控制选项枚举值，例如 OPTION_CABINET_OPEN、OPTION_CABINET_CLOSE；
 *                   若不匹配任何已知命令，返回 OPTION_UNKNOWN
 */
Option_e control_cmd_prase(const char *cmd)
{
    if (strcmp(cmd, "open") == 0)
        return OPTION_CABINET_OPEN;
    else if (strcmp(cmd, "close") == 0)
        return OPTION_CABINET_CLOSE;
    else
        return OPTION_UNKNOWN;
}


/**
 * @brief  根据解析后的 MQTT 控制命令执行对应的柜门操作
 * @param  cmd: 从 MQTT 消息中提取的命令结构体，包含操作类型和柜门位置
 * @retval 无
 *
 * @note   支持的操作包括开门（"open"）和关门（"close"），
 *         其余未知操作将被忽略。
 */

void control_cabinet(ControlJson_t cmd)
{
    Option_e option = control_cmd_prase(cmd.msg.option);

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

/**
 * @brief   将消息加入队列
 * @param   msg: 消息结构体
 * @retval  None
 */
void add_message_to_receive_queue(MQTTMessage_t *msg)
{
    osMessageQueuePut(MQTTMessageReceiveQueueHandle,msg,0,0);
}


