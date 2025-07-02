#include "user_LCDTestTask.h"
#include "lcd.h"
#include "touch.h"
#include "cmsis_os.h"
#include "dwt_delay.h"


/**
 * @brief       清空屏幕并在右上角显示"RST"
 * @param       无
 * @retval      无
 */
void load_draw_dialog(void)
{
    lcd_clear(WHITE);                                                /* 清屏 */
    lcd_show_string(lcddev.width - 24, 0, 200, 16, 16, "RST", BLUE); /* 显示清屏区域 */
}

/**
 * @brief       画粗线
 * @param       x1,y1: 起点坐标
 * @param       x2,y2: 终点坐标
 * @param       size : 线条粗细程度
 * @param       color: 线的颜色
 * @retval      无
 */
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)
        return;

    delta_x = x2 - x1; /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1; /* 设置单步方向 */
    }
    else if (delta_x == 0)
    {
        incx = 0; /* 垂直线 */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0; /* 水平线 */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; /* 选取基本增量坐标轴 */
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) /* 画线输出 */
    {
        lcd_fill_circle(row, col, size, color); /* 画点 */
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/* 10个触控点的颜色(电容触摸屏用) */
const uint16_t POINT_COLOR_TBL[10] = {RED, GREEN, BLUE, BROWN, YELLOW, MAGENTA, CYAN, LIGHTBLUE, BRRED, GRAY};

/**
 * @brief       电容触摸屏测试函数
 * @param       无
 * @retval      无
 */
void ctp_test(void)
{
    uint8_t t = 0;
    uint8_t i = 0;
    uint16_t lastpos[10][2];        /* 最后一次的数据 */
    uint8_t maxp = 5;

    if (lcddev.id == 0x1018)maxp = 10;

    while (1)
    {
        tp_dev.scan(0);

        for (t = 0; t < maxp; t++)
        {
            if ((tp_dev.sta) & (1 << t))
            {
                if (tp_dev.x[t] < lcddev.width && tp_dev.y[t] < lcddev.height)  /* 坐标在屏幕范围内 */
                {
                    if (lastpos[t][0] == 0xFFFF)
                    {
                        lastpos[t][0] = tp_dev.x[t];
                        lastpos[t][1] = tp_dev.y[t];
                    }

                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], tp_dev.x[t], tp_dev.y[t], 2, POINT_COLOR_TBL[t]); /* 画线 */
                    lastpos[t][0] = tp_dev.x[t];
                    lastpos[t][1] = tp_dev.y[t];

                    if (tp_dev.x[t] > (lcddev.width - 24) && tp_dev.y[t] < 20)
                    {
                        load_draw_dialog();/* 清除 */
                    }
                }
            }
            else 
            {
                lastpos[t][0] = 0xFFFF;
            }
        }

        dwt_delay_ms(5);
        i++;

    }
}

void LCDTestTask(void *argument)
{

    dwt_delay_init();

    lcd_init();
    tp_dev.init();

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "TOUCH TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);

    dwt_delay_ms(1500);
    load_draw_dialog();

    ctp_test(); /* 电容屏测试 */
    while (1)
    {
        osDelay(5);
    }
    
}