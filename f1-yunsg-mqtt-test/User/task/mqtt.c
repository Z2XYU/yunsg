#include "tasks.h"
#include "esp8266_mqtt.h"
#include "cmsis_os.h"
void mqtt_task(void* argument)
{
    ESP_Init();
    ESP_MQTT_Connect();
    ESP_MQTT_Subscribe("ysg");

    while (1)
    {
        osDelay(100);
    }
    
}