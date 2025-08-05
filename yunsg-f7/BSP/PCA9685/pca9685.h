#pragma once

#include "main.h"

#define PCA9685_ADDRESS 0x40

#define MODE1_REG   0x00
#define MODE2_REG   0x01

#define PRESCALE_REG    0xFE


extern uint8_t I2C_Transmit_Done;

void PCA9685_Init(void);
void PCA9685_SetFrequency(uint16_t freq);
void PCA9685_SetPWM(uint8_t channel, uint16_t on, uint16_t off);
