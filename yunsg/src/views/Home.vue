<template>
  <div class="home">
    <h1 class="welcome-title">欢迎使用云裳柜</h1>
    <div class="qr-code-container">
      <p class="qr-text">扫描二维码租借：</p>
      <div class="qr-code">
        <canvas ref="qrcodeCanvas" class="qr-canvas"></canvas>
      </div>
    </div>
    <p class="sub-text">探索云裳更多精彩内容</p>
    <div v-if="userInfo" class="user-info">
      <p>欢迎回来，{{ userInfo.name }}！</p>
    </div>
  </div>
</template>

<script>
import QRCode from 'qrcode';
import axios from 'axios';
import { Notification } from 'element-ui';
import { mapState, mapActions } from 'vuex';

export default {
  name: 'Home',
  data() {
    return {
      uid: '', // 生成的UID
    };
  },
  computed: {
    // 使用 Vuex 中的 userInfo
    ...mapState(['userInfo']),
    ...mapState(['closestDeviceId']),
  },
  methods: {
    ...mapActions(['fetchUserInfo']),  // 映射 fetchUserInfo 方法
    ...mapActions(['logout']),

    generateRandomUID() {
      return 'uid_' + Math.random().toString(36).substr(2, 9);
    },
    async generateQRCode() {
      await this.$nextTick(); // 确保DOM更新完成
      // const url = `http://192.168.30.136:8080/login?uid=${this.uid}`;
      const url = `https://www.yunsg.asia/login?uid=${this.uid}`;
      QRCode.toCanvas(this.$refs.qrcodeCanvas, url, (error) => {
        if (error) console.error('二维码生成失败：', error);
      });
    },
    async checkLoginStatus() {
      try {
        const response = await axios.get(`/user/check-login?id=${this.uid}`);

        console.log(this.uid)

        if (response.data.code === 1) {
          // 登录成功，获取用户信息并存储到 Vuex
          this.fetchUserInfo(this.uid); // 使用 Vuex 获取用户信息
          
          // 使用 Element UI 的通知组件
          Notification.success({
            title: '登录成功',
            message: '即将开始探索云裳宝藏',
            duration: 3000
          });
          this.closestDeviceId=26;
          this.$router.push({ name: 'WardrobeDetails', params: { siteID: this.closestDeviceId } });


        } else {

          this.logout();

          setTimeout(() => {
            this.checkLoginStatus(); // 递归检查状态
          }, 3000);
        }
      } catch (error) {
        console.error('检查登录状态失败：', error);
        setTimeout(() => {
          this.checkLoginStatus(); // 递归检查状态
        }, 3000);
      }
    }
  },
  async mounted() {
    this.uid = this.generateRandomUID();
    await this.generateQRCode();
    this.checkLoginStatus();

  }
};
</script>

<style scoped>
/* 页面样式优化 */
html,
body {
  overflow: hidden;
  height: 100%;
}

.home {
  text-align: center;
  background-color: #f5f1e3;
  min-height: 80vh;
  display: flex;
  flex-direction: column;
  justify-content: center;
  padding: 0 15px;
}

.welcome-title {
  font-size: 36px;
  font-weight: bold;
  color: #5f4037;
  margin-bottom: 20px;
  font-family: 'KaiTi', serif;
  animation: fadeIn 1.5s ease-out;
}

.qr-code-container {
  background-color: white;
  border-radius: 16px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.15);
  padding: 25px;
  max-width: 350px;
  margin: 0 auto;
  animation: slideIn 1s ease-out;
}

.qr-text {
  font-size: 18px;
  color: #8b5e3c;
  margin-bottom: 15px;
  font-family: 'KaiTi', serif;
}

.qr-canvas {
  width: 300px;
  height: 300px;
  border-radius: 10px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  animation: scaleUp 1s ease-in-out;
}

.sub-text {
  font-size: 20px;
  color: #7b4b2b;
  margin-top: 20px;
  font-family: 'KaiTi', serif;
  animation: fadeIn 2s ease-out;
}

.user-info {
  margin-top: 20px;
  font-size: 18px;
  color: #5f4037;
  font-family: 'KaiTi', serif;
}

@keyframes fadeIn {
  from {
    opacity: 0;
  }

  to {
    opacity: 1;
  }
}

@keyframes slideIn {
  from {
    transform: translateY(-20px);
    opacity: 0;
  }

  to {
    transform: translateY(0);
    opacity: 1;
  }
}

@keyframes scaleUp {
  from {
    transform: scale(0.5);
    opacity: 0;
  }

  to {
    transform: scale(1);
    opacity: 1;
  }
}
</style>
