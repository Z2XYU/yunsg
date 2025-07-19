#include "env_det_task.h"
#include "sh40.h"
#include "cmsis_os.h"
#include "adc.h"
#include "onenet.h"
#include "4g_d43.h"
#include "stdio.h"
#include "user_TaskInit.h"

SH40_t sh40_snesor;

void SH40Task(void *argument)
{
    while (1)
    {
        sh40_snesor = sh40_measurement_data();
        osThreadFlagsSet(onenetMsgUploadTaskHandle, 0x01);
        printf("tmp:%d hum:%d\n",(int)sh40_snesor.temperature,(int)sh40_snesor.humidity);
        osDelay(5000);
    }
}

static volatile uint16_t sensor_val[SNESOR_NUM] = {0};

void adcSensorsTask(void *argument)
{

    while (1)
    {

        osThreadFlagsClear(0x0001); // 清除标志，防止遗留旧通知

        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)sensor_val, SNESOR_NUM);
        osThreadFlagsWait(0x0001, osFlagsWaitAny, osWaitForever);
        /*数据处理*/

        osDelay(5000);
    }
}

void onenet_upload(void *argument)
{
    if (!d43_init())
    {
        printf("4G模块初始化失败\n");
        osThreadTerminate(onenetMsgUploadTaskHandle);
    }
    printf("4G模块初始化成功\n");

    if (!onenet_connect())
    {
        printf("onenet连接失败\n");
        osThreadTerminate(onenetMsgUploadTaskHandle);
    }
    printf("onenet连接成功\n");
    while (1)
    {
        osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);

        char json[128];
        sprintf(json,
                "{\"temperature\":{\"value\":%.1f},\"humidity\":{\"value\":%.1f}}",
                sh40_snesor.temperature, sh40_snesor.humidity);

        if(onenet_publish(json))
        {
            printf("上传成功\n");
        }
        else
        {
            printf("上传失败\n");
        }

        //osDelay(5000);
    }
}
