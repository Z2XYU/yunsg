import Vue from 'vue';
import VueRouter from 'vue-router';
import Home from '../views/Home.vue';
import WardrobeDetails from '../components/WardrobeDetails.vue';
import ClothingDetails from '../components/ClothingDetails.vue';
import Login from '@/views/Login.vue';
import ReturnClothes from '../components/ReturnClothes.vue'; // 引入还衣服页面组件

Vue.use(VueRouter);

const routes = [
  {
    path: '/',
    name: 'Home',
    component: Home,
  },
  {
    path: '/login',
    name: 'login',   // 登录页面的名称，确保与 App.vue 中的判断匹配
    component: Login  // 登录组件
  },
  {
    path: '/wardrobe/:siteID', // 动态路由，siteID 用于指定不同的共享衣柜
    name: 'WardrobeDetails',
    component: WardrobeDetails,
    props: true, // 将路由参数作为 props 传递给组件
  },
  {
    path: '/clothing/:id', // 动态路由，id 用于指定衣服详情
    name: 'ClothingDetails',
    component: ClothingDetails,
    props: true, // 将路由参数作为 props 传递给组件
  },
  {
    path: '/return-clothes/:siteID',  // 新增的还衣服页面路由
    name: 'ReturnClothes',
    component: ReturnClothes,  // 绑定到刚创建的 ReturnClothes 组件
    props: true, // 将路由参数作为 props 传递给组件
  }
];

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes,
});

export default router;
