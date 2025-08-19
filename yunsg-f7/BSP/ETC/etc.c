#include "etc.h"
#include "pca9685.h"

void etc_init(void)
{
    pca9685_set_pwm(ETC1_CHANNEL,0,0);
    pca9685_set_pwm(ETC2_CHANNEL,0,0);
}

void etc_set_power(uint16_t power)
{
    pca9685_set_pwm(ETC1_CHANNEL,0,power);
    pca9685_set_pwm(ETC2_CHANNEL,0,power);
}

