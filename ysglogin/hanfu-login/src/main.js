import Vue from 'vue'
import App from './App.vue'
import ElementUI from 'element-ui'
import 'element-ui/lib/theme-chalk/index.css' // 导入 Element UI 样式
import Swiper from 'swiper'
import 'swiper/swiper-bundle.css' // 导入 Swiper 样式

Vue.use(ElementUI)

Vue.config.productionTip = false

new Vue({
  render: h => h(App),
}).$mount('#app')
