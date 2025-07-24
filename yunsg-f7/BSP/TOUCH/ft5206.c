#include "string.h"
#include "lcd.h"
#include "touch.h"
#include "ctiic.h"
#include "ft5206.h"
#include "dwt_delay.h"
#include "stdio.h"




uint8_t ft5206_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    ct_iic_start();
    ct_iic_send_byte(FT5206_CMD_WR);    
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0XFF);       
    ct_iic_wait_ack();

    for (i = 0; i < len; i++)
    {
        ct_iic_send_byte(buf[i]);       
        ret = ct_iic_wait_ack();

        if (ret)break;
    }

    ct_iic_stop();  
    return ret;
}


void ft5206_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    ct_iic_start();
    ct_iic_send_byte(FT5206_CMD_WR);    
    ct_iic_wait_ack();
    ct_iic_send_byte(reg & 0XFF);      
    ct_iic_wait_ack();
    ct_iic_start();
    ct_iic_send_byte(FT5206_CMD_RD);    
    ct_iic_wait_ack();

    for (i = 0; i < len; i++)
    {
        buf[i] = ct_iic_read_byte(i == (len - 1) ? 0 : 1);  
    }

    ct_iic_stop();  
}


uint8_t ft5206_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    uint8_t temp[2];

    FT5206_RST_GPIO_CLK_ENABLE();   
    FT5206_INT_GPIO_CLK_ENABLE();  

    gpio_init_struct.Pin = FT5206_RST_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            
    gpio_init_struct.Pull = GPIO_PULLUP;                    
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     
    HAL_GPIO_Init(FT5206_RST_GPIO_PORT, &gpio_init_struct); 

    gpio_init_struct.Pin = FT5206_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                
    gpio_init_struct.Pull = GPIO_PULLUP;                    
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     
    HAL_GPIO_Init(FT5206_INT_GPIO_PORT, &gpio_init_struct); 

    ct_iic_init();      
    FT5206_RST(0);      
    dwt_delay_ms(20);
    FT5206_RST(1);      
    dwt_delay_ms(50);
    temp[0] = 0;
    ft5206_wr_reg(FT5206_DEVIDE_MODE, temp, 1); 
    ft5206_wr_reg(FT5206_ID_G_MODE, temp, 1);   
    temp[0] = 22;                               
    ft5206_wr_reg(FT5206_ID_G_THGROUP, temp, 1);
    temp[0] = 12;                               
    ft5206_wr_reg(FT5206_ID_G_PERIODACTIVE, temp, 1);
    

    ft5206_rd_reg(FT5206_ID_G_LIB_VERSION, &temp[0], 2);

    if ((temp[0] == 0X30 && temp[1] == 0X03) || temp[1] == 0X01 || temp[1] == 0X02 || (temp[0] == 0x0 && temp[1] == 0X0))   /* ?��:0X3003/0X0001/0X0002/CST340 */
    {
        printf("CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
        return 0;
    }

    return 1;
}

/* FT5206 5???????? ??????????? */
const uint16_t FT5206_TPX_TBL[5] = {FT5206_TP1_REG, FT5206_TP2_REG, FT5206_TP3_REG, FT5206_TP4_REG, FT5206_TP5_REG};

/**
 * @brief       ??��????(???��?????)
 * @param       mode : ??????��????��???, ???????????
 * @retval      ?????????
 *   @arg       0, ?????????; 
 *   @arg       1, ?????��???;
 */
uint8_t ft5206_scan(uint8_t mode)
{
    uint8_t sta = 0;
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint16_t temp;
    static uint8_t t = 0;   /* ?????????,???????CPU????? */
    
    t++;
    
    if ((t % 10) == 0 || t < 10)   /* ?????,?????10??CTP_Scan????????1??,??????CPU????? */
    {
        ft5206_rd_reg(FT5206_REG_NUM_FINGER, &sta, 1);  /* ???????????? */

        if ((sta & 0XF) && ((sta & 0XF) < 6))
        {
            temp = 0XFFFF << (sta & 0XF);           /* ????????????1??��??,???tp_dev.sta???? */
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;

            dwt_delay_ms(4);    /* ??????????????????????��??????? */

            for (i = 0; i < 5; i++)
            {
                if (tp_dev.sta & (1 << i))          /* ??????��? */
                {
                    ft5206_rd_reg(FT5206_TPX_TBL[i], buf, 4);   /* ???XY????? */

                    if (tp_dev.touchtype & 0X01)    /* ???? */
                    {
                        tp_dev.y[i] = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
                        tp_dev.x[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
                    }
                    else
                    {
                        tp_dev.x[i] = lcddev.width - (((uint16_t)(buf[0] & 0X0F) << 8) + buf[1]);
                        tp_dev.y[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
                    }

                    if ((buf[0] & 0XF0) != 0X80)tp_dev.x[i] = tp_dev.y[i] = 0;      /* ??????contact????????????�� */

                    //printf("x[%d]:%d,y[%d]:%d\r\n", i, tp_dev.x[i], i, tp_dev.y[i]);
                }
            }

            res = 1;

            if (tp_dev.x[0] == 0 && tp_dev.y[0] == 0)
            {
                sta = 0;   /* ?????????????0,??????????? */
            }

            t = 0;  /* ???????,??????????????10??,???????????? */
        }
    }

    if ((sta & 0X1F) == 0)  /* ????????? */
    {
        if (tp_dev.sta & TP_PRES_DOWN)      /* ?????????? */
        {
            tp_dev.sta &= ~TP_PRES_DOWN;    /* ????????? */
        }
        else    /* ??????��????? */
        {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE000;           /* ???????��??? */
        }
    }

    if (t > 240)t = 10; /* ?????10??????? */

    return res;
}



























