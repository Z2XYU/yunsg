#include "temperature_control_tasks.h"
#include "sh40.h"
#include "pca9685.h"
#include "ptc.h"
#include "etc.h"
#include "pid.h"
#include "cmsis_os.h"
#include "fan.h"

#define TARGET_TEMPERATURE 20

// 0 代表制冷模式 1代表加热模式
static uint8_t control_mode = 0;

SH40_t sh40_sensor;
PID_Controller ETC_pid, PTC_pid;

void temperatureControlTask(void *argument)
{
    pca9685_init();
    ptc_init();
    etc_init();
    pid_init(&ETC_pid, 500, 100, 0, TARGET_TEMPERATURE, 10);
    pid_init(&PTC_pid, 500, 100, 0, TARGET_TEMPERATURE, 10);

    while (1)
    {
        sh40_sensor = sh40_measurement_data();
        float current_temperature = sh40_sensor.temperature;
        float current_time = osKernelGetTickCount() / (float)osKernelGetTickFreq();

        if ((current_temperature < TARGET_TEMPERATURE - 3) && control_mode != 1)
        {
            control_mode = 1;
            pid_clear(&PTC_pid);
            pid_clear(&ETC_pid);
        }
        else if ((current_temperature > TARGET_TEMPERATURE + 3) && control_mode != 0)
        {
            control_mode = 0;
            pid_clear(&PTC_pid);
            pid_clear(&ETC_pid);
        }

        if ((current_temperature <= TARGET_TEMPERATURE + 3) && (current_temperature >= TARGET_TEMPERATURE - 3))
        {
            fan_disable();
        }
        else
        {
            fan_enable();
        }

        // 计算 PID 输出
        float power = 0, pid_output = 0;

        switch (control_mode)
        {
        case 0:
            pid_output = pid_computer(&ETC_pid, current_temperature, current_time);
            // 更新功率
            power = pid_output;
            etc_set_power(power);
            break;

        case 1:
            pid_output = pid_computer(&PTC_pid, current_temperature, current_time);
            // 更新功率
            power = pid_output;
            ptc_set_power(power);
            break;

        default:
            break;
        }

        osDelay(10000);
    }
}