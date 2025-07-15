#include "drivers/register/stc8g.h"
#include "drivers/uart/uart.h"
#include <utils/delay/soft_delay.h>
#include <drivers/ssd1306/ssd1306.h>
#include <drivers/i2c/i2c.h>
#include <drivers/ina226/ina226.h>
#include <stdint.h>

#define OLED_HIDE_TIME 15000

static int oled_display_time = 0;
static __bit sleep           = 0;

static float voltage = 0;
static float current = 0;
static float power   = 0;

static void ports_init(void)
{
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;

    // 设置P3.2 为 INA226的Alert 输入
    P3M0 |= 0x04;
    P3M1 |= 0x04;
}

static void interrupts_init(void)
{
    EA = 1;
}

static void key_init(void)
{
    EX1 = 1;
    IT1 = 1;
}

void oled_hide(void)
{
    ssd1306_display_off();
    PCON |= 0x02; // 电源控制寄存器
}

void oled_show(void)
{
    PCON &= ~0x02;
    ssd1306_display_on();
}

void key_interrupt(void) __interrupt(2)
{
    if (sleep) {
        oled_show();
        sleep = 0;
        TR0   = 1;
    }
}

void Timer0_Init(void) // 1毫秒@24.000MHz
{
    AUXR |= 0x80; // 定时器时钟1T模式
    TMOD &= 0xF0; // 设置定时器模式
    TL0 = 0x40;   // 设置定时初始值
    TH0 = 0xA2;   // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时
    ET0 = 1;      // 使能定时器0中断
}

void Timer0_Isr(void) __interrupt(1)
{
    if (oled_display_time++ >= OLED_HIDE_TIME) {
        sleep             = 1;
        oled_display_time = 0;
        TR0               = 0;
    }
}

void main(void)
{
    ports_init();      // 端口配置
    interrupts_init(); // 中断开启
    uart_init();       // 初始化串口

      

    uart_send_string("hello world\n");
    i2c_init();
    uart_send_string("i2c init success\n");
    ina226_init();
    uart_send_string("ina226 init success\n");
    ssd1306_init();
    uart_send_string("ssd1306 init success\n");
    

    ssd1306_clear();
    ssd1306_draw_str(0, 0, (uint8_t *)"Hello wrold!", 8);
    Delay100ms();
    ssd1306_draw_str(0, 2, (uint8_t *)"STC8G @24MHz", 8);
    ssd1306_draw_str(0, 3, (uint8_t *)"BatteryBoard 2.0", 8);
    for (int i = 0; i < 1; i++)
        Delay1s();
    ssd1306_clear();


    Timer0_Init();
    key_init();

    /* 绘制静态UI：测量值文字信息（固定区域） */
    ssd1306_draw_str(0, 0, (uint8_t *)"V:", 8);  // 电压信息，在页0（y=0~7）
    ssd1306_draw_str(70, 0, (uint8_t *)"A:", 8); // 电流信息
    ssd1306_draw_str(0, 2, (uint8_t *)"W:", 8);  // 功率信息，在页2（y=16~23）
    ssd1306_draw_str(70, 2, (uint8_t *)"L:", 8); // 负载百分比

    while (1) 
    {
        if (sleep) 
            oled_hide();

        
        voltage = ina226_read_volt();
        current = ina226_read_current();
        power   = ina226_read_power();

        /* 每次循环更新测量值显示
       （注意：此处简单调用绘制函数，假设新数值完全覆盖旧值） */
        ssd1306_draw_float(20, 0, voltage, 2, 8);
        ssd1306_draw_float(90, 0, current, 2, 8);
        ssd1306_draw_float(20, 2, power, 2, 8);
        ssd1306_draw_float(90, 2, (voltage / 12.0f) * 100.0f, 2, 8);

        uart_send_uint16_t((int)(voltage * 10.0f));

        

        Delay10ms();
    }
}
