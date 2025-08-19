#pragma once

#include "main.h"

#define PCA9685_ADDRESS 0x40

#define MODE1_REG   0x00
#define MODE2_REG   0x01

#define PRESCALE_REG    0xFE


void pca9685_set_frequency(uint16_t freq);
void pca9685_init(void);
void pca9685_set_pwm(uint8_t channel, uint16_t on, uint16_t off);
