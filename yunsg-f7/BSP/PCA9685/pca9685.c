#include "pca9685.h"
#include "i2c.h"
#include "cmsis_os.h"

// PCA9685 I2C 写数据

volatile uint8_t I2C_Transmit_Done = 0;

void PCA9685_WriteReg(uint8_t reg, uint8_t data)
{
    uint8_t txData[2];

    txData[0] = reg;
    txData[1] = data;

    I2C_Transmit_Done = 0;

    if (HAL_I2C_Master_Transmit_IT(&hi2c1, PCA9685_ADDRESS << 1, txData, 2) != HAL_OK)
    {
        Error_Handler();
    }

    while (I2C_Transmit_Done == 0)
    {
        osDelay(1);
    }
}

void PCA9685_SetFrequency(uint16_t freq)
{
    uint8_t prescaler;

    // 计算分频系数值
    prescaler = (uint8_t)(25000000 / (4096 + freq) - 1);

    // 进入睡眠模式设置频率
    PCA9685_WriteReg(MODE1_REG, 0x80);
    // 设置分频器
    PCA9685_WriteReg(PRESCALE_REG, prescaler);
    // 退出睡眠模式
    PCA9685_WriteReg(MODE1_REG, 0xA1);
}

// 设置PCA9685 PWM 占空比
void PCA9685_SetPWM(uint8_t channel, uint16_t on, uint16_t off)
{
    uint8_t data[5];

    uint8_t base_addr = 0x06 + 4 * channel;

    data[0] = base_addr;
    data[1] = on & 0xff;
    data[2] = on >> 8;
    data[3] = off & 0xff;
    data[4] = off >> 8;

    // 重置标志位
    I2C_Transmit_Done = 0;

    if (HAL_I2C_Master_Transmit_IT(&hi2c1, PCA9685_ADDRESS << 1, data, 5) != HAL_OK)
    {
        Error_Handler();
    }

    // 等待传输完成
    while (I2C_Transmit_Done == 0)
    {
        osDelay(1);
    }
}

void PCA9685_Init(void)
{
    PCA9685_SetFrequency(1000);
}