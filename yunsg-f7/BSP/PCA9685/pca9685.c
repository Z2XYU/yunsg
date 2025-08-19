#include "pca9685.h"
#include "cmsis_os.h"
#include "myiic.h"



static void pca9685_write_reg(uint8_t reg, uint8_t data)
{
    iic_start();
    iic_send_byte(PCA9685_ADDRESS << 1);
    iic_wait_ack();
    iic_send_byte(reg);
    iic_wait_ack();
    iic_send_byte(data);
    iic_wait_ack();
    iic_stop();
}

void pca9685_set_frequency(uint16_t freq)
{
    uint8_t prescaler;

    // 计算分频系数值
    prescaler = (uint8_t)(25000000 / (4096 + freq) - 1);

    // 进入睡眠模式设置频率
    pca9685_write_reg(MODE1_REG, 0x80);
    // 设置分频器
    pca9685_write_reg(PRESCALE_REG, prescaler);
    // 退出睡眠模式
    pca9685_write_reg(MODE1_REG, 0xA1);
}

void pca9685_init(void)
{
    pca9685_set_frequency(1000);
}


void pca9685_set_pwm(uint8_t channel, uint16_t on, uint16_t off)
{
    uint8_t base_addr = 0x06 + 4 * channel;

    iic_start();
    iic_send_byte(PCA9685_ADDRESS << 1); // 写模式
    iic_wait_ack();

    iic_send_byte(base_addr);             // 寄存器地址
    iic_wait_ack();

    iic_send_byte(on & 0xFF);             // ON 低字节
    iic_wait_ack();
    iic_send_byte(on >> 8);               // ON 高字节
    iic_wait_ack();
    iic_send_byte(off & 0xFF);            // OFF 低字节
    iic_wait_ack();
    iic_send_byte(off >> 8);              // OFF 高字节
    iic_wait_ack();

    iic_stop();
}

