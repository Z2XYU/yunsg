#include "etc.h"
#include "pca9685.h"

void ETC_Init(void)
{
    PCA9685_SetPWM(ETC1_CHANNEL,0,0);
    PCA9685_SetPWM(ETC2_CHANNEL,0,0);
}

void ETC_SetPower(uint16_t power)
{
    PCA9685_SetPWM(ETC1_CHANNEL,0,power);
    PCA9685_SetPWM(ETC2_CHANNEL,0,power);
}

