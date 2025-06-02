<template>
  <div id="app">
    <!-- 只有不是登录页面时才显示导航栏和页脚 -->
    <div v-if="!isLoginPage">
      <!-- 顶部导航栏 -->
      <header class="app-header">
        <h1>云裳柜</h1>
        <nav>
          <router-link to="/">首页</router-link>
          <!-- 动态链接到最近设备的页面 -->
          <!-- <router-link :to="`/wardrobe/${closestDeviceId}`">探索云裳宝藏</router-link>
          <router-link :to="`/return-clothes/${closestDeviceId}`">归衣轩</router-link> -->


          <router-link :to="`/wardrobe/13`">探索云裳宝藏</router-link>
          <router-link :to="`/return-clothes/13`">归衣轩</router-link>
        </nav>

        <!-- 右上角显示用户信息 -->
        <div v-if="userInfo" class="user-info">
          <p class="welcome-text">欢迎使用云裳柜，{{ userInfo.name }}！</p>
        </div>
      </header>
    </div>

    <!-- 主体内容区域 -->
    <main>
      <router-view></router-view> <!-- 根据路由动态显示内容 -->
    </main>

    <!-- 页脚 -->
    <div v-if="!isLoginPage">
      <footer class="app-footer">
        <p>版权所有 © 2025 共享衣柜</p>
      </footer>
    </div>
  </div>
</template>

<script>
import axios from 'axios';
import { mapState } from 'vuex';

export default {
  name: 'App',
  data() {
    return {
      isLoginPage: false,  // 默认为 false，表示不是登录页面
      devices: [],
      closestDeviceId: null,
      currentLocation: { latitude: null, longitude: null },  // 当前位置信息
    };
  },
  computed: {
    // 映射 Vuex 中的 userInfo 状态
    ...mapState(['userInfo'])
  },
  watch: {
    // 监听路由变化，判断当前是否是登录页面
    $route(to) {
      this.isLoginPage = to.name === 'login';  // 如果是登录页面，设置为 true
    }
  },
  created() {
    // 初始化时判断是否是登录页面
    this.isLoginPage = this.$route.name === 'login';
    this.getTerminalList();
    this.getCurrentLocation();  // 获取当前位置
  },

  methods: {
    // 获取当前位置的经纬度
    async getCurrentLocation() {
      if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(
          (position) => {
            this.currentLocation.latitude = position.coords.latitude;
            this.currentLocation.longitude = position.coords.longitude;
            this.updateClosestDeviceId();  // 更新最近设备 ID
          },
          (error) => {
            console.error('获取位置失败', error);
          }
        );
      } else {
        console.log('浏览器不支持地理定位');
      }
    },

    // 更新最近的设备 ID，确保设备列表已加载完成
    async updateClosestDeviceId() {
      if (this.devices.length > 0 && this.currentLocation.latitude && this.currentLocation.longitude) {
        this.closestDeviceId = this.getClosestDeviceId();  // 获取最近设备 ID
        console.log(this.closestDeviceId);
      }
    },

    async getTerminalList() {
      this.isLoading = true; // 设置加载状态
      try {
        const response = await axios.get('/site/terminal');
        if (response && response.data) {
          this.devices = response.data.data;
          console.log(this.devices);
          this.updateClosestDeviceId();  // 在设备列表加载完成后更新最近设备 ID
        } else {
          console.error('终端数据为空或格式不正确');
          this.devices = [];
        }
      } catch (error) {
        console.error('获取终端列表失败:', error);
        this.devices = [];
      } finally {
        this.isLoading = false;
      }
    },

    // Haversine 公式计算距离
    calculateDistance(lat1, lon1, lat2, lon2) {
      const R = 6371; // 地球半径，单位为 km
      const dLat = this.degreesToRadians(lat2 - lat1);
      const dLon = this.degreesToRadians(lon2 - lon1);
      const a = Math.sin(dLat / 2) * Math.sin(dLat / 2) +
        Math.cos(this.degreesToRadians(lat1)) * Math.cos(this.degreesToRadians(lat2)) *
        Math.sin(dLon / 2) * Math.sin(dLon / 2);
      const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
      return R * c; // 返回距离，单位为 km
    },

    // 将角度转化为弧度
    degreesToRadians(degrees) {
      return degrees * (Math.PI / 180);
    },

    // 获取距离当前用户最近的设备 ID
    getClosestDeviceId() {
      if (!this.currentLocation.latitude || !this.currentLocation.longitude) {
        return null;  // 如果当前位置未获取到，返回 null
      }

      let closestId = null;
      let minDistance = Infinity;

      this.devices.forEach(device => {
        const distance = this.calculateDistance(
          this.currentLocation.latitude,
          this.currentLocation.longitude,
          device.latitude,
          device.longitude
        );

        if (distance < minDistance) {
          minDistance = distance;
          closestId = device.cabinetId;
        }
      });
      console.log(closestId);
      // 使用 Vuex 保存 closestDeviceId
      this.$store.dispatch('setClosestDeviceId', closestId);
      return closestId;
    }
  }

};
</script>

<style scoped>
#app {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
  font-family: 'KaiTi', serif;
  /* 使用楷体字体，增加古风感 */
  background-color: #f9f6f1;
  /* 古风淡雅背景 */
  margin: 0;
}

.app-header {
  background-color: #8b5e3c;
  /* 古风棕色调 */
  color: white;
  padding: 12px 16px;
  text-align: center;
  font-size: 20px;
  border-bottom: 2px solid #663300;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  width: 100%;
  position: relative;
}

.app-header nav {
  margin-top: 8px;
}

.app-header a {
  color: white;
  margin: 0 15px;
  text-decoration: none;
  font-weight: bold;
  font-size: 18px;
}

.app-header a:hover {
  text-decoration: underline;
}

/* 右上角显示用户信息 */
.user-info {
  position: absolute;
  top: 20px;
  /* 增加距离顶部的空间 */
  right: 20px;
  /* 增加距离右侧的空间 */
  font-size: 18px;
  /* 放大字体 */
  color: white;
  text-align: right;
  font-weight: bold;
  /* 增加粗体字效果 */
}

.welcome-text {
  font-size: 20px;
  /* 设置欢迎文本的字体大小 */
  margin: 0;
}

.user-info .user-avatar {
  width: 35px;
  /* 放大头像 */
  height: 35px;
  border-radius: 50%;
  margin-left: 10px;
}

/* 主体内容区域 */
main {
  flex: 1;
  padding: 20px;
  margin-top: 10px;
  margin-bottom: 10px;
  overflow-y: auto;
}

.app-footer {
  background-color: #8b5e3c;
  color: white;
  text-align: center;
  padding: 10px;
  border-top: 2px solid #663300;
  box-shadow: 0 -4px 6px rgba(0, 0, 0, 0.1);
  width: 100%;
}

/* 响应式设计：为小屏幕优化布局 */
@media (max-width: 768px) {
  .app-header nav a {
    font-size: 16px;
    margin: 0 10px;
  }

  .app-footer p {
    font-size: 14px;
  }

  .user-info {
    font-size: 16px;
    /* 在小屏幕上缩小字体 */
    top: 15px;
    right: 15px;
  }
}
</style>
