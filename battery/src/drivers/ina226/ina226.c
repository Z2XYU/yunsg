#include <drivers/ina226/ina226.h>
#include <drivers/i2c/i2c.h>
#include "ina226_config.h"
void ina226_init(void)
{
    uint8_t config_high = 0x41;
    uint8_t config_low  = 0x27;

    uint8_t calib_high = INA226_CALIBRATION >> 8;
    uint8_t calib_low  = INA226_CALIBRATION & 0xFF;

    // write Configuration Registe
    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 0);  // Write mode
    i2c_recv_ack();
    i2c_send_data(0x00);                  // Pointer to configuration register
    i2c_recv_ack();
    i2c_send_data(config_high);           // High byte of configuration
    i2c_recv_ack();
    i2c_send_data(config_low);            // Low byte of configuration
    i2c_recv_ack();
    i2c_stop();

    // write Calibration Register
    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 0);  // Write mode
    i2c_recv_ack();
    i2c_send_data(0x05);                  // Pointer to calibration register
    i2c_recv_ack();
    i2c_send_data(calib_high);            // High byte of calibration value
    i2c_recv_ack();
    i2c_send_data(calib_low);             // Low byte of calibration value
    i2c_recv_ack();
    i2c_stop();
}

float ina226_read_volt(void)
{
    uint8_t high, low;
    uint16_t raw;
    float voltage_value;

    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 0);  // Write mode
    i2c_recv_ack();
    i2c_send_data(0x02);                  // Pointer to bus voltage register
    i2c_recv_ack();
    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 1);  // Read mode
    i2c_recv_ack();
    high = i2c_recv_data();
    i2c_send_ack();
    low = i2c_recv_data();
    i2c_send_nack();
    i2c_stop();

    raw           = ((uint16_t)high << 8) | low;
    voltage_value = raw * 0.00125;
    return voltage_value;
}

float ina226_read_current(void)
{
    uint8_t high, low;
    signed int raw;
    float current_value;

    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 0);
    i2c_recv_ack();
    i2c_send_data(0x04);
    i2c_recv_ack();
    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 1);
    i2c_recv_ack();
    high = i2c_recv_data();
    i2c_send_ack();
    low = i2c_recv_data();
    i2c_send_nack();
    i2c_stop();

    raw = (signed int)((high << 8) | low);
    current_value = raw * 0.001;
    return current_value;
}

float ina226_read_power(void)
{
    uint8_t high, low;
    uint16_t raw;
    float power_value;

    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 0);
    i2c_recv_ack();
    i2c_send_data(0x03);
    i2c_recv_ack();
    i2c_start();
    i2c_send_data(INA226_ADDR << 1 | 1);
    i2c_recv_ack();
    high = i2c_recv_data();
    i2c_send_ack();
    low = i2c_recv_data();
    i2c_send_nack();
    i2c_stop();

    raw         = ((uint16_t)high << 8) | low;
    power_value = raw * 0.025;
    return power_value;
}
