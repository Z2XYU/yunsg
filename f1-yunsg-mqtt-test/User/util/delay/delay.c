// SPDX-License-Identifier: GPL-3.0-only
/*
 * Copyright (c) 2008-2023 100askTeam : Dongshan WEI <weidongshan@qq.com> 
 * Discourse:  https://forums.100ask.net
 */

 
/*  Copyright (C) 2008-2023 深圳百问网科技有限公司
 *  All rights reserved
 *
 *
 * 免责声明: 百问网编写的文档，仅供学员学习使用，可以转发或引用(请保留作者信息)，禁止用于商业用途！
 * 免责声明: 百问网编写的程序，可以用于商业用途，但百问网不承担任何后果！
 * 
 * 
 * 本程序遵循GPL V3协议，使用请遵循协议许可
 * 本程序所用的开发板：	DShanMCU-F103
 * 百问网嵌入式学习平台：https://www.100ask.net
 * 百问网技术交流社区：	https://forums.100ask.net
 * 百问网官方B站：				https://space.bilibili.com/275908810
 * 百问网官方淘宝：			https://100ask.taobao.com
 * 联系我们(E-mail)：	  weidongshan@qq.com
 *
 * 版权所有，盗版必究。
 *  
 * 修改历史     版本号           作者        修改内容
 *-----------------------------------------------------
 * 2023.08.04      v01         百问科技      创建文件
 *-----------------------------------------------------
 */


#include "delay.h"
#include "stm32f1xx_hal.h"

/**********************************************************************
 * 函数名称： udelay
 * 功能描述： us级别的延时函数(复制rt-thread的代码)
 * 输入参数： us - 延时多少us
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/08/03	     V1.0	  韦东山	      创建
 ***********************************************************************/
void udelay(int us)
{
#if 0    
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (1000);  /* 假设reload对应1ms */
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
#else
    extern TIM_HandleTypeDef        htim1;
    TIM_HandleTypeDef *hHalTim = &htim1;

    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = __HAL_TIM_GET_AUTORELOAD(hHalTim);

    ticks = us * reload / (1000);  /* 假设reload对应1ms */
    told = __HAL_TIM_GET_COUNTER(hHalTim);
    while (1)
    {
        tnow = __HAL_TIM_GET_COUNTER(hHalTim);
        if (tnow != told)
        {
            if (tnow > told)
            {
                tcnt += tnow - told;
            }
            else
            {
                tcnt += reload - told + tnow;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
#endif
}


/**********************************************************************
 * 函数名称： mdelay
 * 功能描述： ms级别的延时函数
 * 输入参数： ms - 延时多少ms
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/08/03	     V1.0	  韦东山	      创建
 ***********************************************************************/
void mdelay(int ms)
{
    for (int i = 0; i < ms; i++)
        udelay(1000);
}



/**********************************************************************
 * 函数名称： system_get_ns
 * 功能描述： 获得系统时间(单位ns)
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 系统时间(单位ns)
 * 修改日期        版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/08/03	     V1.0	  韦东山	      创建
 ***********************************************************************/
uint64_t system_get_ns(void)
{
    //extern uint32_t HAL_GetTick(void);
    extern TIM_HandleTypeDef        htim4;
    TIM_HandleTypeDef *hHalTim = &htim4;
    
    uint64_t ns = HAL_GetTick();
    uint64_t cnt;
    uint64_t reload;

    cnt = __HAL_TIM_GET_COUNTER(hHalTim);
    reload = __HAL_TIM_GET_AUTORELOAD(hHalTim);

    ns *= 1000000;
    ns += cnt * 1000000 / reload;
    return ns;
}


