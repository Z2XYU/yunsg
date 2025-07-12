#include "stdio.h"
#include "sh40.h"
#include "myiic.h"
#include "dwt_delay.h"

/**
 * @brief  计算 SHT40 温湿度传感器数据的 CRC8 校验值
 * @param  data 指向待校验的2字节数据的指针
 * @retval 计算得到的 CRC8 校验值
 * @note   使用多项式 x^8 + x^5 + x^4 + 1 (0x31)，初始值 0xFF
 */
static uint8_t sht_crc8(uint8_t *data)
{
    uint8_t crc = 0xFF;
    for (uint8_t i = 0; i < 2; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)                 //crc 判断第8位是否为1
                crc = (crc << 1) ^ 0x31;    //与多项式值 0x31 异或
            else
                crc <<= 1;
        }
    }
    return crc;
}

/**
 * @brief  读取 SHT40 温湿度传感器数据
 * @retval SH40_t 结构体，包含温度和湿度（带 CRC 校验）
 *
 * 步骤说明：
 * 1. 发送测量命令到 SHT40
 * 2. 延时等待测量完成（典型 9ms）
 * 3. 读取温度和湿度原始数据及 CRC 校验字节
 * 4. 对温湿度数据进行 CRC 校验，校验通过则转换为实际物理值，否则返回错误标志
 */
SH40_t sh40_measurement_data()
{
    SH40_t sensor = {0};

    // 第一步：发送测量命令
    iic_start();
    iic_send_byte(SH40_WRITE); // 写模式
    iic_wait_ack();
    iic_send_byte(SH40_MEASURE_CMD); // 测量命令
    iic_wait_ack();
    iic_stop();

    // 第二步：延时等待测量完成（典型 9ms）
    dwt_delay_ms(9);

    // 第三步：读取数据
    iic_start();
    iic_send_byte(SH40_READ); // 读模式
    iic_wait_ack();
    uint8_t temperature_H = iic_read_byte(1); // 读取温度高字节
    uint8_t temperature_L = iic_read_byte(1); // 读取温度低字节
    uint8_t ccr1 = iic_read_byte(1);          // 读取温度 CRC 校验字节
    uint8_t humidity_H = iic_read_byte(1);    // 读取湿度高字节
    uint8_t humidity_L = iic_read_byte(1);    // 读取湿度低字节
    uint8_t ccr2 = iic_read_byte(0);          // 读取湿度 CRC 校验字节（最后一字节不应发送 ACK）
    iic_stop();

    // 第四步：CRC 校验
    uint8_t temp_raw[2] = {temperature_H, temperature_L};
    uint8_t humi_raw[2] = {humidity_H, humidity_L};

    // 校验温度数据 CRC
    if (sht_crc8(temp_raw) != ccr1)
    {
        // 温度 CRC 错误处理（返回错误标志值）
        sensor.temperature = -999.0f;
    }
    else
    {
        // 温度数据转换为实际温度值（单位：摄氏度）
        uint16_t raw_temp = ((uint16_t)temperature_H << 8) | temperature_L;
        sensor.temperature = -45.0f + 175.0f * ((float)raw_temp / 65535.0f);
    }

    // 校验湿度数据 CRC
    if (sht_crc8(humi_raw) != ccr2)
    {
        // 湿度 CRC 错误处理（返回错误标志值）
        sensor.humidity = -999.0f;
    }
    else
    {
        // 湿度数据转换为实际湿度值（单位：%RH）
        uint16_t raw_humi = ((uint16_t)humidity_H << 8) | humidity_L;
        sensor.humidity = -6.0f + 125.0f * ((float)raw_humi / 65535.0f);
    }

    return sensor;
}

/**
 * @brief  读取 SHT40 传感器的序列号
 * @retval 32位序列号
 *
 * 步骤说明：
 * 1. 发送读取序列号命令到 SHT40
 * 2. 延时等待数据准备
 * 3. 读取4字节序列号数据
 * 4. 合成为32位序列号返回
 */
uint32_t sh40_read_serial_number(void)
{
    uint32_t serial_number;

    uint32_t data[4];
    iic_start();

    iic_send_byte(SH40_WRITE);      // 发送写地址
    iic_wait_ack();
    iic_send_byte(SH40_SERIAL_NUMBER); // 发送读取序列号命令
    iic_wait_ack();

    dwt_delay_ms(10);               // 等待数据准备

    iic_start();
    iic_send_byte(SH40_READ);       // 发送读地址
    iic_wait_ack();

    data[0] = iic_read_byte(1);     // 读取序列号高字节
    data[1] = iic_read_byte(1);     // 读取序列号次高字节
    data[2] = iic_read_byte(1);     // 读取序列号次低字节
    data[3] = iic_read_byte(0);     // 读取序列号低字节（最后一字节不应发送 ACK）

    iic_stop();

    // 合成32位序列号
    serial_number = ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3]);
    return serial_number;
}
