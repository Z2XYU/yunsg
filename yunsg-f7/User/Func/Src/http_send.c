#include "http_send.h"
#include "cJSON.h"
#include "string.h"
#include "usart.h"
#include "stdlib.h"
#include "stdio.h"

char *pickup_code_msg_packing(PickupCode_t *code)
{
    cJSON *root = cJSON_CreateObject();


    cJSON_AddNumberToObject(root,"site_id",code->site_id);
    cJSON_AddStringToObject(root, "pickup_code", code->pickup_code);

    char *json_str = cJSON_PrintUnformatted(root);

    strcat(json_str, "@");

    cJSON_Delete(root);

    return json_str;
}

void pickup_code_send(char *json_str)
{
    if (json_str != NULL)
    {
        HAL_UART_Transmit_DMA(&huart2, (uint8_t *)json_str, strlen(json_str));
    }
}