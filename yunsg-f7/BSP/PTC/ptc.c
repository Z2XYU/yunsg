#include "ptc.h"
#include "pca9685.h"

void ptc_init(void)
{
    pca9685_set_pwm(PTC1_CHANNEL,0,0);
    pca9685_set_pwm(PTC2_CHANNEL,0,0);
}

void ptc_set_power(uint16_t power)
{
    pca9685_set_pwm(PTC1_CHANNEL,0,power);
    pca9685_set_pwm(PTC2_CHANNEL,0,power);
}



