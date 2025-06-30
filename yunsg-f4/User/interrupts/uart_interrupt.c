#include "main.h"
#include "tasks.h"
#include "string.h"
#include "stdio.h"

extern DMA_HandleTypeDef hdma_usart2_rx;
uint8_t wifi_rx_buffer[WIFI_RX_BUFFER_SIZE];

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart->Instance==USART2)
    {
        //HAL_UART_Transmit_DMA(huart,wifi_rx_buffer,Size);
        
        //拷贝接收数据
        wifiMessage_t msg;
        memcpy(msg.data,wifi_rx_buffer,Size);
        msg.length=Size;

        //加入消息队列
        osMessageQueuePut(wifiMsgQueueHandle,&msg,0,0);

        //开始接收
        HAL_UARTEx_ReceiveToIdle_DMA(huart,wifi_rx_buffer,sizeof(wifi_rx_buffer));
        __HAL_DMA_DISABLE_IT(&hdma_usart2_rx,DMA_IT_HT);
    }
}