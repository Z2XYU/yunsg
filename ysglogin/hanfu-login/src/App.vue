<template>
  <div id="app" class="login-page">
    <div class="background">
      <div class="login-container">
        <h2 class="login-title">用户登录</h2>
        <el-form ref="form" :model="form" status-icon>
          <el-form-item label="用户名" :rules="[{ required: true, message: '请输入用户名', trigger: 'blur' }]">
            <el-input v-model="form.username" placeholder="请输入用户名" />
          </el-form-item>

          <el-form-item label="密码" :rules="[{ required: true, message: '请输入密码', trigger: 'blur' }]">
            <el-input type="password" v-model="form.password" placeholder="请输入密码" />
          </el-form-item>

          <el-form-item>
            <el-button type="primary" @click="handleLogin" class="login-btn">登录</el-button>
          </el-form-item>
        </el-form>
      </div>
    </div>
    <div class="footer">
      <a href="https://beian.miit.gov.cn/" target="_blank">豫ICP备2024085959号-1</a>
    </div>
  </div>
</template>

<script>
export default {
  name: 'App',
  data() {
    return {
      form: {
        username: '',
        password: ''
      }
    };
  },
  methods: {
    handleLogin() {
      console.log('用户名:', this.form.username);
      console.log('密码:', this.form.password);
      // 在这里处理登录逻辑
    }
  }
};
</script>

<style scoped>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body, html {
  height: 100%;
  width: 100%;
  font-family: 'Arial', sans-serif;
}

#app {
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  height: 100vh;
  width: 100%;
  background: linear-gradient(135deg, #e0eafc, #cfdef3);
  background-size: cover;
  background-attachment: fixed;
  overflow: hidden;
}

.background {
  background-color: rgba(255, 255, 255, 0.85);
  padding: 30px;
  border-radius: 15px;
  box-shadow: 0 15px 30px rgba(0, 0, 0, 0.2);
  max-width: 450px;
  text-align: center;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
}

.login-container {
  background: #f7f7f7;
  padding: 35px;
  border-radius: 15px;
  box-shadow: 0 8px 24px rgba(0, 0, 0, 0.1);
  width: 100%;
}

.login-title {
  font-size: 38px;
  font-weight: 600;
  color: #2c3e50;
  margin-bottom: 20px;
  font-family: 'KaiTi', serif;
}

.el-form-item label {
  font-size: 14px;
  color: #2c3e50;
}

.el-button.login-btn {
  background: linear-gradient(135deg, #4CAF50, #45a049);
  border: none;
  width: 100%;
  font-size: 16px;
  padding: 12px;
  margin-top: 20px;
  border-radius: 10px;
}

.el-button.login-btn:hover {
  background: linear-gradient(135deg, #45a049, #4CAF50);
}

.footer {
  position: absolute;
  bottom: 10px;
  width: 100%;
  text-align: center;
  font-size: 14px;
  color: #333;
}

.footer a {
  color: #666;
  text-decoration: none;
  margin: 0 10px;
}

.footer a:hover {
  color: #000;
}

@media (max-width: 768px) {
  .login-title {
    font-size: 32px;
  }

  .el-form-item label {
    font-size: 12px;
  }

  .el-button.login-btn {
    font-size: 14px;
    padding: 10px;
  }

  .background {
    padding: 15px;
    max-width: 90%;
  }
}
</style>
