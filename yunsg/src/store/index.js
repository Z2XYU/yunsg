import Vue from 'vue';
import Vuex from 'vuex';
import axios from 'axios'; // 确保已安装并导入 axios

Vue.use(Vuex);

// 创建 axios 实例，设置基础 URL
const api = axios.create({
  baseURL: 'https://www.yunsc.asia/',  // 默认的基础 URL
  timeout: 10000,  // 请求超时时间
});

export default new Vuex.Store({
  state: {
    userInfo: JSON.parse(localStorage.getItem('userInfo')) || null,  // 从 localStorage 获取用户信息
    wardrobeData: {}, // 存储共享衣柜数据
    loading: false,  // 请求状态标记
    closestDeviceId: null, // 添加 closestDeviceId 全局变量
  },
  mutations: {
    // 设置用户信息
    SET_USER_INFO(state, userInfo) {
      state.userInfo = userInfo;
      localStorage.setItem('userInfo', JSON.stringify(userInfo));  // 更新时同步到 localStorage
    },
    // 设置共享衣柜数据
    setWardrobeData(state, data) {
      state.wardrobeData = data;
    },
    // 清除用户信息
    clearUserInfo(state) {
      state.userInfo = null;
      localStorage.removeItem('userInfo');  // 清除时移除 localStorage 中的用户信息
    },
    // 设置请求加载状态
    setLoading(state, status) {
      state.loading = status;
    },
    // 清除衣柜数据
    clearWardrobeData(state) {
      state.wardrobeData = {};
    },
    // 设置 closestDeviceId
    SET_CLOSEST_DEVICE_ID(state, deviceId) {
      state.closestDeviceId = deviceId; // 更新 closestDeviceId
    },
    // 登出
    LOGOUT(state) {
      state.userInfo = null;
      sessionStorage.removeItem('uid'); // 清除 sessionStorage 中的 uid
    }
  },
  actions: {
    // 获取用户信息
    async fetchUserInfo({ commit }, uid) {
      try {
        const response = await axios.get(`/user/yunsg/userinfo?id=${uid}`);
        if (response.data.code === 1) {
          commit('SET_USER_INFO', response.data.data); // 获取并提交到 Vuex 状态
        }
      } catch (error) {
        console.error('获取用户信息失败：', error);
      }
    },
    // 注销用户
    logout({ commit }) {
      commit('LOGOUT'); // 调用 LOGOUT mutation 清除用户信息
    },
    // 获取共享衣柜数据
    async fetchWardrobeData({ commit }, siteID) {
      commit('setLoading', true);  // 开始请求时设置 loading 状态
      try {
        const response = await api.get(`siteterminal/hanfu/${siteID}`);
        commit('setWardrobeData', response.data); // 请求成功后存储数据
      } catch (error) {
        console.error('Error fetching wardrobe data:', error); // 请求失败时输出错误
      } finally {
        commit('setLoading', false);  // 请求完成后重置 loading 状态
      }
    },
    // 设置 closestDeviceId
    setClosestDeviceId({ commit }, deviceId) {
      commit('SET_CLOSEST_DEVICE_ID', deviceId); // 调用 mutation 来更新 closestDeviceId
    }
  },
  getters: {
    // 判断用户是否已登录
    isLoggedIn: (state) => !!state.userInfo,
    // 获取共享衣柜数据
    wardrobeData: (state) => state.wardrobeData,
    // 获取用户信息
    userInfo: (state) => state.userInfo,
    // 获取加载状态
    isLoading: (state) => state.loading,
    // 获取 closestDeviceId
    closestDeviceId: (state) => state.closestDeviceId, // 获取 closestDeviceId
  }
});
