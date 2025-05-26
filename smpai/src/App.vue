<template>
  <div id="app">
    <h1>控制树莓派GPIO</h1>
    <div class="button-group">
      <button @click="sendCommand('on')" :disabled="isLoading">打开LED</button>
      <button @click="sendCommand('off')" :disabled="isLoading">关闭LED</button>
    </div>

    <!-- 状态提示 -->
    <div v-if="message" class="message" :class="messageType">
      {{ message }}
    </div>

    <!-- 加载状态 -->
    <div v-if="isLoading" class="loading">发送指令中...</div>
  </div>
</template>

<script>
export default {
  data() {
    return {
      backendUrl: 'http://192.168.249.252:5000/control', // 请替换为实际的树莓派 IP
      isLoading: false, // 加载状态
      message: '', // 提示信息
      messageType: 'info', // 提示类型：info/error
    };
  },
  methods: {
    async sendCommand(command) {
      this.isLoading = true;
      this.message = '';

      try {
        const response = await fetch(this.backendUrl, {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json',
          },
          body: JSON.stringify({ command }),
        });

        if (!response.ok) {
          throw new Error('网络响应异常');
        }

        const data = await response.json();
        this.message = `操作成功: ${JSON.stringify(data)}`;
        this.messageType = 'info';
      } catch (error) {
        this.message = `错误: ${error.message}`;
        this.messageType = 'error';
      } finally {
        this.isLoading = false;
      }
    },
  },
};
</script>

<style scoped>
#app {
  font-family: Arial, sans-serif;
  max-width: 600px;
  margin: 0 auto;
  padding: 20px;
  text-align: center;
}

h1 {
  color: #2c3e50;
}

.button-group {
  margin: 20px 0;
}

button {
  padding: 10px 20px;
  margin: 5px;
  font-size: 16px;
  cursor: pointer;
  border: none;
  border-radius: 5px;
  background-color: #3498db;
  color: white;
  transition: background-color 0.3s;
}

button:disabled {
  background-color: #bdc3c7;
  cursor: not-allowed;
}

button:hover:not(:disabled) {
  background-color: #2980b9;
}

.message {
  padding: 10px;
  margin-top: 20px;
  border-radius: 5px;
}

.message.info {
  background-color: #d5f5e3;
  color: #27ae60;
}

.message.error {
  background-color: #f8d7da;
  color: #dc3545;
}

.loading {
  margin-top: 20px;
  font-style: italic;
  color: #7f8c8d;
}
</style>