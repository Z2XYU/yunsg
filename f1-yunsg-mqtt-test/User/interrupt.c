#include "main.h"
#include "esp8266.h"
extern uint8_t receive_data;
extern uint8_t esp8266_rx_buf[ESP8266_RX_BUF_SIZE];
extern uint8_t esp8266_tx_buf[ESP8266_TX_BUF_SIZE];
extern uint16_t esp8266_cnt;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        if (esp8266_cnt >= sizeof(esp8266_rx_buf))
            esp8266_cnt = 0;
        esp8266_rx_buf[esp8266_cnt++] = receive_data;
        // HAL_UART_Transmit_IT(huart,&receive_data,1);


        HAL_UART_Receive_IT(huart, &receive_data, 1); // 重新启动中断接收
    }
}