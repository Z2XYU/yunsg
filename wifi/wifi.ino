#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include <ESP8266HTTPClient.h>

//连接WIFI
const char* ssid = SSID;
const char* password = PASSWORD;

//MQTT 服务器消息
const char* mqtt_server = MQTT_SERVER;

//初始化客户端
WiFiClient esp_client;
PubSubClient client(esp_client);

//LED引脚
const int LED_PIN = 2;

//串口接收缓冲区
String serialBuffer = "";



void setup_wifi() {
  delay(10);
  Serial.println("连接WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(1000);
    Serial.print(".");
  }

  digitalWrite(LED_PIN, LOW);
  Serial.println("WiFi连接成功");
}

void callback(char* topic, byte* payload, unsigned int length) {
  digitalWrite(LED_PIN, LOW);
  Serial.print(topic);
  Serial.print("-");


  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  delay(200);
  digitalWrite(LED_PIN, HIGH);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("正在连接MQTT服务器...");


    String client_id = "ESP8266Client-" + String(random(0xffff), HEX);

    if (client.connect(client_id.c_str())) {
      Serial.println("已连接");
      digitalWrite(LED_PIN, HIGH);
      client.subscribe("control");  //订阅控制主题
    } else {
      Serial.print("连接失败...,rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}


void handleSerialJson(const String& jsonStr) {
  StaticJsonDocument<256> doc;  // 注意增大容量适应嵌套结构

  DeserializationError error = deserializeJson(doc, jsonStr);
  if (error) {
    //Serial.println("JSON解析失败");
    return;
  }

  const char* topic = doc["topic"];
  JsonObject msg = doc["msg"];

  if (topic && !msg.isNull()) {
    const char* option = msg["option"];
    const char* value = msg["value"];

    if (option && value) {
      StaticJsonDocument<128> sendDoc;
      sendDoc["option"] = option;
      sendDoc["value"] = value;
      char buffer[128];
      serializeJson(sendDoc, buffer);
      client.publish(topic, buffer);
    }

  } else {
    //Serial.println("JSON字段不完整");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  WiFiClient client;
  HTTPClient http;

  http.begin(client, "http://192.168.137.1:8889/act");  // 请求URL
  int httpCode = http.GET();                              // 发送GET请求

  if (httpCode == 200) {
    String payload = http.getString();
    Serial.print("Payload length: ");
    Serial.println(payload.length());
    Serial.println("Payload content:");
    Serial.println(payload);
    Serial.flush();
  } else {
    Serial.print("HTTP error: ");
    Serial.println(httpCode);
  }
  http.end();  // 关闭连接
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  //串口接收和缓存
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '!') {
      handleSerialJson(serialBuffer);

      serialBuffer = "";  //清空缓存
    } else {
      serialBuffer += c;
    }
  }
}
