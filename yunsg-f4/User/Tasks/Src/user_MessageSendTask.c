#include "user_MessageSendTask.h"
#include "cmsis_os.h"
#include "user_TaskInit.h"
#include "message_send.h"
#include "stdio.h"

void MessageSendTask(void *argument)
{
    while (1)
    {
        VoiceJson_t voice_json;
        if (osMessageQueueGet(MQTTMessageSendQueueHandle, &voice_json, NULL, osWaitForever) == osOK)
        {
            char* json_str= message_package(&voice_json);
            message_send(json_str);

            //printf("%s\n",json_str);
        }
    }
}