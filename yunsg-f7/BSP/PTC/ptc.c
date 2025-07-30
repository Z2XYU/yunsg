#include "ptc.h"
#include "pca9685.h"

void PTC_Init(void)
{
    PCA9685_SetPWM(PTC1_CHANNEL,0,0);
    PCA9685_SetPWM(PTC2_CHANNEL,0,0);
}



