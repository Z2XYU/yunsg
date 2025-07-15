#include "uart.h"
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 8
static char buf[BUFFER_SIZE];

//115200bps @24.000MHz
void uart_init(void)
{
    SCON = 0x50;        //8位数据，可变波特率
    AUXR |= 0x40;       //定时器时钟1T模式
    AUXR &= 0xFE;       //串口1选择定时器1为波特率发生器

    TMOD = 0x0F;        //设置定时器模式
    TL1 = 0xCC;         //设置定时器初值
    TH1 = 0xFF;         //设置定时器初值
    ET1 = 0;            //禁止定时器1中断
    TR1 = 1;            //定时器1开始计时
}

// 发送单个字节
void uart_send_byte(uint8_t byte)
{
    SBUF=byte;
    while (!TI);
    TI=0;
}

//发送字符串（以'\0'为结束标志）
void uart_send_string(const char *str)
{
    while (*str)
    {
        uart_send_byte(*str++);
    }
}

//发送一个无符号整数（转换为字符串后发送）
void uart_send_uint16_t(uint16_t num)
{
    memset(buf,0,BUFFER_SIZE);
    sprintf(buf,"%u",num);
    uart_send_string(buf);
}

//发送浮点数（格式为小数点后2位）
void uart_send_float(float num)
{
    memset(buf,0,BUFFER_SIZE);
    sprintf(buf,"%.2f",num);
    uart_send_string(buf);
}