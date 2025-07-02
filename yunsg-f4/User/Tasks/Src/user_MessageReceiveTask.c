#include "user_TaskInit.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "usart.h"
#include "cJSON.h"
#include "string.h"

#include "mqttMessageProcess.h"

extern uint8_t mqtt_rx_buffer[MQTT_MESSAGE_BUFFER_SIZE];
extern DMA_HandleTypeDef hdma_usart2_rx;

/**
 * @brief  MQTT 消息接收任务
 * @param  argument: 任务参数，未使用
 * @retval None
 *
 * @details
 * 该任务通过 UART DMA 接收 MQTT 消息，
 * 并从消息队列中获取消息进行解析和处理，
 * 调用对应的控制函数执行业务逻辑。
 */
void MessageReceiveTask(void *argument)
{
     HAL_UARTEx_ReceiveToIdle_DMA(&huart2, mqtt_rx_buffer, sizeof(mqtt_rx_buffer));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
    while (1)
    {
        MQTTMessage_t msg;
        if (osMessageQueueGet(MQTTMessageQueueHandle, &msg, NULL, osWaitForever) == osOK)
        {
            //printf("已接受消息\n");
            MQTTJson_t control_cmd = MQTT_Message_Parse(msg);

            MQTT_Cabinet_Control(control_cmd);

            // HAL_UART_Transmit_DMA(&huart2, msg.data, msg.length);
        }
    }
    
}