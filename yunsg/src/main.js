// main.js
import Vue from 'vue';
import App from './App.vue';
import router from './router';
import store from './store';
import axios from 'axios';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';

Vue.use(ElementUI);
Vue.config.productionTip = false;

// 设置全局 Axios 默认配置
Vue.prototype.$axios = axios;
axios.defaults.baseURL = 'https://www.yunsc.asia'; // 替换为实际后端 API 地址

// axios.defaults.baseURL = 'http://129.211.26.112:8889'; // 替换为实际后端 API 地址

// axios.defaults.baseURL = 'http://172.24.218.174:8889';
axios.defaults.timeout = 10000; // 设置请求超时时间

new Vue({
  router,
  store,
  render: (h) => h(App),
}).$mount('#app');