<template>
    <div class="login-form">
      <h1>欢迎踏入汉服的世界</h1>
      <form @submit.prevent="handleLogin">
        <div class="input-group">
          <label for="username">用户名</label>
          <input type="text" id="username" v-model="username" placeholder="请输入用户名" />
        </div>
        <div class="input-group">
          <label for="password">密码</label>
          <input type="password" id="password" v-model="password" placeholder="请输入密码" />
        </div>
        <button type="submit" class="login-btn">登录</button>
      </form>
    </div>
  </template>
  
  <script>
  export default {
    data() {
      return {
        username: '',
        password: ''
      };
    },
    methods: {
      handleLogin() {
        if (this.username && this.password) {
          alert('登录成功');
          // 执行登录逻辑
        } else {
          alert('请输入用户名和密码');
        }
      }
    }
  }
  </script>
  
  <style scoped>
  .login-form {
    background: rgba(255, 255, 255, 0.8);
    padding: 30px;
    border-radius: 10px;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
    text-align: center;
  }
  
  h1 {
    font-family: 'KaiTi', sans-serif;
    font-size: 28px;
    color: #C85A54;
    margin-bottom: 20px;
  }
  
  .input-group {
    margin-bottom: 20px;
    text-align: left;
  }
  
  .input-group label {
    font-family: 'KaiTi', sans-serif;
    font-size: 16px;
    color: #4C3C31;
  }
  
  input {
    width: 100%;
    padding: 10px;
    border: 2px solid #C85A54;
    border-radius: 5px;
    font-size: 16px;
  }
  
  button {
    background-color: #C85A54;
    color: white;
    padding: 10px 20px;
    border: none;
    border-radius: 5px;
    font-size: 18px;
    cursor: pointer;
  }
  
  button:hover {
    background-color: #9E4A3B;
  }
  </style>
  