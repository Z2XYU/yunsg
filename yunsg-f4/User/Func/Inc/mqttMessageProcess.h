#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MQTT_MESSAGE_BUFFER_SIZE 128  // MQTT消息缓冲区大小

// MQTT消息结构体，包含数据和长度
typedef struct 
{
    uint8_t data[MQTT_MESSAGE_BUFFER_SIZE]; // 消息数据
    uint16_t length;                        // 数据长度
} MQTTMessage_t;

// 控制消息结构体，包含动作、选项和柜子位置
typedef struct
{
    char action[32];        // 动作字段
    char option[32];        // 选项字段
    int cabinetLocation;    // 柜子位置
} ControlMessage_t;

// 解析后的MQTT JSON结构体，包含主题和控制消息
typedef struct 
{   
    char topic[32];             // 主题
    ControlMessage_t msg;       // 控制消息
} MQTTJson_t;

// 控制命令枚举
typedef enum
{
  OPTION_UNKNOWN=0,         // 未知命令
  OPTION_CABINET_OPEN,      // 开柜命令
  OPTION_CABINET_CLOSE      // 关柜命令
} Option_e;

// 解析MQTT消息为结构体
MQTTJson_t MQTT_Message_Parse(MQTTMessage_t msg);

// 控制柜子动作
void MQTT_Cabinet_Control(MQTTJson_t cmd);

#ifdef __cplusplus
}
#endif
