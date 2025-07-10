#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"



typedef struct
{
    char option[16];        // 选项字段
    char value[64];
}VoicMessage_t;

//语音消息结构体
typedef struct
{
    char topic[16];
    VoicMessage_t msg;
}VoiceJson_t;

void add_message_to_send_queue(VoiceJson_t *json);
void message_send(char *json_str);

char* message_package(VoiceJson_t *json);

#ifdef __cplusplus
}
#endif