<template>
  <div class="login-page">
    <h2>登录</h2>
    <el-form ref="form" :model="form" status-icon>
      <el-form-item label="用户名" prop="username">
        <el-input v-model="form.username" placeholder="请输入用户名" class="input-field" />
      </el-form-item>

      <el-form-item label="密码" prop="password">
        <el-input v-model="form.password" type="password" placeholder="请输入密码" class="input-field" />
      </el-form-item>

      <el-form-item>
        <el-button type="primary" @click="handleLogin" class="login-btn">登录</el-button>
      </el-form-item>
    </el-form>
  </div>
</template>

<script>
import axios from 'axios';
import { mapState } from 'vuex';
export default {
  name: 'Login',
  data() {
    return {
      form: {
        username: '',
        password: ''
      },

      uid: this.$route.query.uid // 获取 URL 中的 uid
    };
  },
  computed:{
    ...mapState(['closestDeviceId']),
  },
  methods: {
    async handleLogin() {
      try {
        const response = await axios.post(`/user/login/yunsg/${this.uid}`, {
          account: this.form.username,
          password: this.form.password,
        });

        if (response.data.code === 1) {
          // 登录成功后提示信息，不进行页面跳转
          this.$message.success('登录成功');
        } else {
          this.$message.error(response.data.message || '用户名或密码错误');
        }
      } catch (error) {
        this.$message.error('登录失败，请稍后再试');
        console.error('登录请求失败：', error);
      }
    }
  }
};
</script>

<style scoped>
.login-page {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  height: 100vh;
  background: linear-gradient(135deg, #f5d0a9, #d9a99d); /* 背景渐变 */
  padding: 20px;
  box-sizing: border-box;
}

h2 {
  font-size: 36px;
  font-weight: bold;
  color: #5f4037;
  margin-bottom: 20px;
  text-align: center;
  text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1); /* 文字阴影 */
}

.el-form-item {
  margin-bottom: 20px;
  width: 100%;
  max-width: 400px; /* 最大宽度为400px */
}

.input-field {
  border-radius: 8px; /* 输入框圆角 */
  background-color: #fdfdfd; /* 背景色柔和的浅米色 */
  border: 1px solid #c6a48b; /* 边框颜色与整体色调协调 */
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1); /* 输入框阴影效果 */
  padding: 10px;
}

.el-input__inner {
  font-size: 16px; /* 输入框字体 */
}

.el-button {
  width: 100%;
  font-size: 18px;
  padding: 12px;
  background-color: #8b5e3c; /* 按钮背景色 */
  border: none;
  border-radius: 8px; /* 按钮圆角 */
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); /* 按钮阴影 */
  transition: background-color 0.3s, transform 0.2s ease-in-out; /* 按钮过渡效果 */
}

.el-button:hover {
  background-color: #663300; /* 悬停时按钮颜色变化 */
  transform: scale(1.05); /* 悬停时按钮放大 */
}

/* 响应式设计：为不同设备优化布局 */
@media (max-width: 768px) {
  h2 {
    font-size: 28px;
  }

  .el-form-item label {
    font-size: 14px;
  }

  .el-button {
    padding: 10px;
    font-size: 16px;
  }

  .login-page {
    padding: 15px; /* 小屏幕上减少内边距 */
  }
}
</style>
