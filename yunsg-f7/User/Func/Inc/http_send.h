/*后期将http 和 mqtt 数据使用联合体整合*/
#pragma once


#include "main.h"

typedef struct
{
    int site_id;
    char pickup_code[32];
}PickupCode_t;


char* pickup_code_msg_packing(PickupCode_t *code);

void pickup_code_send(char *json_str);

