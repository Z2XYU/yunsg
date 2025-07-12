#pragma once

#include "main.h"

#define SH40_ADDRESS 0x44
#define SH40_MEASURE_CMD  0xFD // 高精度测量命令
#define SH40_SERIAL_NUMBER 0x89 //读取唯一序列号命令

#define SH40_WRITE  (0x44<<1)
#define SH40_READ   ((0x44<<1)|1)

typedef struct 
{   
    float temperature;
    float humidity;    
}SH40_t;


SH40_t sh40_measurement_data();
uint32_t sh40_read_serial_number(void);
