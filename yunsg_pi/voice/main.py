import paho.mqtt.client as mqtt
import asyncio
import os
from edge_tts import Communicate
import json
import configparser

config =configparser.ConfigParser()
config_path = os.path.join(os.path.dirname(__file__), 'config.ini')

config.read(config_path)

MQTT_BROKER = config['MQTT']['BROKER']
MQTT_PORT = int(config['MQTT']['PORT'])
MQTT_TOPIC = config['MQTT']['TOPIC']

BASE_DIR = os.path.dirname(os.path.abspath(__file__))  
SAVE_DIR = os.path.join(BASE_DIR, "audio_resources")
os.makedirs(SAVE_DIR, exist_ok=True)

async def save_numbers():
    for i in range(1, 10):
        text = str(i)  
        filepath = os.path.join(SAVE_DIR, f"{text}.mp3")
        if not os.path.exists(filepath):
            print(f"生成音频：{filepath}")
            communicate = Communicate(text=text, voice="zh-CN-XiaoxiaoNeural")
            await communicate.save(filepath)
        else:
            print(f"音频已存在，跳过：{filepath}")

    filepath=os.path.join(SAVE_DIR,f"*.mp3")
    if not os.path.exists(filepath):
        communicate = Communicate(text="删除", voice="zh-CN-XiaoxiaoNeural")
        await communicate.save(filepath)
    else:
        print(f"音频已存在，跳过：{filepath}")

    filepath=os.path.join(SAVE_DIR,f"#.mp3")
    if not os.path.exists(filepath):
        communicate = Communicate(text="柜门已打开，请及时取走衣物", voice="zh-CN-XiaoxiaoNeural")
        await communicate.save(filepath)
    else:
        print(f"音频已存在，跳过：{filepath}")
    

def on_connect(client, userdata, flags, rc):
    print("已连接，返回码：", rc)
    client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
    json_str = msg.payload.decode()
    print(f"[收到的消息] Topic: {msg.topic}, Payload: {json_str}")

    data = json.loads(json_str)

    if(data['option']=="button"):
        filepath = os.path.join(SAVE_DIR, f"{data['value']}.mp3")
        if os.path.exists(filepath):
            os.system(f'mpg123 "{filepath}"')
        else:
            print(f"找不到音频文件：{filepath}")
    else:
        print("暂不处理")
    

def main():
    asyncio.run(save_numbers())

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(MQTT_BROKER, MQTT_PORT, 60)
    client.loop_forever()

if __name__ == "__main__":
    main()
