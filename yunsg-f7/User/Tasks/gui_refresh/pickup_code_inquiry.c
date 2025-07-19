#include "pickup_code_inquiry.h"
#include "http_send.h"
#include "user_TaskInit.h"
#include "stdio.h"

void pickupCodeInquiryTask(void *argument)
{
    PickupCode_t pick;
    while (1)
    {
        if(osMessageQueueGet(PickupCodeMsgSendQueueHandle,&pick,NULL,osWaitForever)==osOK)
        {
            char* pick_str = pickup_code_msg_packing(&pick);
            
            //printf("pick_str:%s\n",pick_str);
            osDelay(500);

            pickup_code_send(pick_str);
        }
    }
}