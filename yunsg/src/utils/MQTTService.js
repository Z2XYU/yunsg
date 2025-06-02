// src/utils/MQTTService.js
import mqtt from 'mqtt/dist/mqtt.min.js'  // 推荐使用浏览器版以避免 webpack 问题

class MQTTService {
  constructor(connectUrl, options) {
    this.connectUrl = connectUrl;
    this.options = options;
    this.client = null;
  }

  connect() {
    this.client = mqtt.connect(this.connectUrl, this.options);

    this.client.on('connect', () => {
      console.log('✅ MQTT 已连接');
    });

    this.client.on('reconnect', (error) => {
      console.log('🔄 正在重连:', error);
    });

    this.client.on('error', (error) => {
      console.error('❌ 连接失败:', error);
      this.client.end();
    });

    this.client.on('message', (topic, message) => {
      console.log('📩 收到消息:', topic, message.toString());
      if (this.onMessageCallback) {
        this.onMessageCallback(topic, message.toString());
      }
    });
  }

  // 订阅
  subscribe(topic) {
    if (this.client) {
      this.client.subscribe(topic, (err) => {
        if (err) {
          console.error('❌ 订阅失败:', err);
        } else {
          console.log(`✅ 已订阅: ${topic}`);
        }
      });
    }
  }

  // 发布
  publish(topic, message) {
    if (this.client) {
      this.client.publish(topic, message);
      console.log(`🚀 已发布到 ${topic}: ${message}`);
    }
  }

  // 设置收到消息的回调
  onMessage(callback) {
    this.onMessageCallback = callback;
  }

  // 断开连接
  disconnect() {
    if (this.client) {
      this.client.end();
      console.log('🛑 已断开 MQTT');
    }
  }
}

export default MQTTService;
