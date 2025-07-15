#include "rc522.h"
#include "rfid_compare.h"
#include "stdio.h"


uint8_t readUid[5]; // 卡号
uint8_t CT[3];      // 卡类型
uint8_t DATA[16];   // 存放数据

uint8_t KEY_A[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t KEY_B[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

uint8_t status;
uint8_t addr = 0x01; // 0扇区 0块

void Cardcompare(int id)
{
    uint8_t i;

    status = RC522_Request(0x52, CT); // 找到卡返回0
    if (!status)                    // 寻卡成功
    {
        status = RC522_ERR;
        status = RC522_Anticoll(readUid); // 防冲撞
    }

    if (!status) // 防冲撞成功
    {
        status = RC522_ERR;
        printf("卡的类型为：%x%x%x\r\n", CT[0], CT[1], CT[2]); // 读取卡的类型 //读取卡的类型
        printf("卡号：%x-%x-%x-%x\r\n", readUid[0], readUid[1], readUid[2], readUid[3]);
        //HAL_Delay(1000);
        status = RC522_Select(readUid);
    }

    if (!status) // 选卡成功
    {
        status = RC522_ERR;
        // 验证A密钥 块地址 密码 SN
        status = RC522_AuthState(0x60, addr, KEY_A, readUid);
        if (status == RC522_OK) // 验证A成功
        {
            printf("A密钥验证成功\r\n");
            //HAL_Delay(1000);
        }
        else
        {
            printf("A密钥验证失败\r\n");

            //HAL_Delay(1000);
        }

        // 验证B密钥 块地址 密码 SN
        status = RC522_AuthState(0x61, addr, KEY_B, readUid);
        if (status == RC522_OK) // 验证B成功
        {
            printf("B密钥验证成功\r\n");
        }
        else
        {
            printf("B密钥验证失败\r\n");

        }
        //HAL_Delay(1000);
    }

    if (status == RC522_OK) // 验证密码成功，接着读取0块
    {
        status = RC522_ERR;
        status = RC522_ReadBlock(addr, DATA);
        if (status == RC522_OK) // 读卡成功
        {
            printf("0扇区0块DATA:");
            for (i = 0; i < 16; i++)
            {
                printf("%02x", DATA[i]);
            }
            printf("\r\n");

            /*数据处理*/

        }
        else
        {
            printf("读卡失败\r\n");

        }
    }

}