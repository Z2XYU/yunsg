#include "esp8266_mqtt.h"
#include "string.h"
#include "stdio.h"

// 外部串口句柄，需在 main.c 中初始化
extern UART_HandleTypeDef huart1; // 用于串口调试（printf 输出）
extern UART_HandleTypeDef huart2; // 用于与 ESP8266 通信

// 打印调试信息到串口1（串口调试口）
static void Debug_Print(const char *msg)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), 100);
}

// 向 ESP8266 发送 AT 命令并延时等待
static void ESP_SendAT(const char *cmd, uint16_t delay_ms)
{
    char buffer[128];
    sprintf(buffer, "%s\r\n", cmd);                                      // 添加回车换行
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), 1000); // 发送到 ESP8266
    Debug_Print(">> ");
    Debug_Print(buffer); // 在调试口打印发送的 AT 命令
    HAL_Delay(delay_ms); // 等待 ESP 返回响应
}

// 初始化 ESP8266 模块并连接 WiFi 与 MQTT 服务
void ESP_Init(void)
{
    ESP_SendAT("AT", 1000);                                              // 测试模块是否响应
    ESP_SendAT("AT+CWMODE=1", 500);                                      // 设置为 Station 模式
    ESP_SendAT("AT+CWJAP=\"玩会吧，别真考上了\",\"18568601031\"", 5000); // 连接 WiFi（SSID + 密码）
    ESP_SendAT("AT+CIPMUX=0", 500);                                      // 单连接模式
    ESP_SendAT("AT+CIPSTART=\"TCP\",\"129.211.26.112\",1883", 3000);     // 连接 MQTT 服务器 1883 端口
}

// 向服务器发送 MQTT CONNECT 报文，建立 MQTT 会话
void ESP_MQTT_Connect(void)
{
    // 构造 MQTT CONNECT 报文（协议版本 3.1.1，保持连接 60s，客户端ID为 test）
    uint8_t mqtt_connect[] = {
        0x10, 0x12,                     // 固定报头：CONNECT 报文，剩余长度 0x12（18）
        0x00, 0x04, 'M', 'Q', 'T', 'T', // 协议名 "MQTT"
        0x04,                           // 协议版本号 4 (MQTT 3.1.1)
        0x02,                           // 连接标志（Clean Session）
        0x00, 60,                       // KeepAlive 时间：60秒
        0x00, 0x04, 't', 'e', 's', 't'  // 客户端ID：test
    };

    char cmd[32];
    sprintf(cmd, "AT+CIPSEND=%d", sizeof(mqtt_connect)); // 设置即将发送的数据长度
    ESP_SendAT(cmd, 100);
    HAL_UART_Transmit(&huart2, mqtt_connect, sizeof(mqtt_connect), 1000); // 发送 MQTT CONNECT 报文
    Debug_Print("<< MQTT CONNECT Sent\r\n");
    HAL_Delay(1000); // 等待服务器返回连接确认报文 CONNACK
}

void ESP_MQTT_Subscribe(const char* topic) {
    uint16_t topic_len = strlen(topic);
    uint8_t packet[256] = {0};
    uint8_t i = 0;

    packet[i++] = 0x82; // MQTT订阅报文固定头：订阅报文，QoS=1，报文类型0x8（订阅），低4位0010
    // 剩余长度计算：2(消息ID) + 2(主题长度) + 主题长度 + 1(QoS)
    packet[i++] = 2 + 2 + topic_len + 1;

    // 消息ID，随便设个1
    packet[i++] = 0x00;
    packet[i++] = 0x01;

    // 主题长度
    packet[i++] = (topic_len >> 8) & 0xFF;
    packet[i++] = topic_len & 0xFF;

    // 主题字符串
    memcpy(&packet[i], topic, topic_len);
    i += topic_len;

    // QoS等级，0或1，这里写1
    packet[i++] = 0x01;

    char cmd[32];
    sprintf(cmd, "AT+CIPSEND=%d", i);
    ESP_SendAT(cmd, 100);
    HAL_UART_Transmit(&huart2, packet, i, 1000);

    Debug_Print("<< MQTT SUBSCRIBE Sent: ");
    Debug_Print(topic);
    Debug_Print("\r\n");

    HAL_Delay(500);
}


// 发布 MQTT 消息到指定主题
void ESP_MQTT_Publish(const char *topic, const char *message)
{
    uint16_t topic_len = strlen(topic);
    uint16_t msg_len = strlen(message);
    uint8_t packet[256] = {0};

    uint8_t i = 0;
    packet[i++] = 0x30;                    // 固定报头，Publish 报文（QoS 0，非保留）
    packet[i++] = 2 + topic_len + msg_len; // 剩余长度：主题长度字段 + 主题内容 + 消息内容
    packet[i++] = topic_len >> 8;          // 主题长度高字节
    packet[i++] = topic_len & 0xFF;        // 主题长度低字节
    memcpy(&packet[i], topic, topic_len);
    i += topic_len; // 填充主题
    memcpy(&packet[i], message, msg_len);
    i += msg_len; // 填充消息内容

    char cmd[32];
    sprintf(cmd, "AT+CIPSEND=%d", i); // 设置即将发送的 MQTT PUBLISH 报文长度
    ESP_SendAT(cmd, 100);
    HAL_UART_Transmit(&huart2, packet, i, 1000); // 发送 MQTT PUBLISH 报文

    Debug_Print("<< MQTT PUBLISH Sent: ");
    Debug_Print(message);
    Debug_Print("\r\n");

    HAL_Delay(500); // 稍作延迟，等待服务器处理
}
