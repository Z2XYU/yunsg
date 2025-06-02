<template>
  <div class="return-clothes">
    <div v-if="hanfuDetails.length" class="orders-container">
      <div
        v-for="(order, index) in hanfuDetails"
        :key="index"
        class="order-card"
      >
        <!-- 展示汉服详情 -->
        <div class="hanfu-details">
          <h3>订单详情</h3>
          <div v-if="order.hanfu" class="hanfu-info">
            <div class="hanfu-img-container">
              <img :src="order.hanfu.image" alt="hanfu" class="hanfu-img" />
            </div>
            <div class="hanfu-text-info">
              <p><strong>汉服名称：</strong>{{ order.hanfu.name }}</p>
              <p><strong>租金：</strong>{{ order.hanfu.rent }} /天</p>
              <p><strong>价格：</strong>{{ order.hanfu.price }}</p>
              <p><strong>商铺名称：</strong>{{ order.hanfu.shopName }}</p>
              <p><strong>租赁时间：</strong>{{ order.startTime }}</p>
            </div>
          </div>
        </div>
        <button @click="confirmReturn(order)">确认归还</button>
        <p v-if="order.message" class="message">{{ order.message }}</p>
      </div>
    </div>
    <p v-else class="no-orders">暂无待归还订单</p>

    <!-- 弹窗显示归还结果 -->
    <!-- 弹窗显示归还结果 -->
    <div v-if="showModal" class="modal-overlay">
      <div class="modal-content">
        <div class="modal-body">
          <p>订单已成功归还！</p>
          <p>
            <strong>总花费：{{ totalCost }} 元</strong>
          </p>
          <p>
            <strong>{{ location }}号柜门已打开。</strong>
          </p>
          <!-- 显示柜门号 -->
        </div>
        <div class="modal-footer">
          <button @click="closeModal" class="btn-close">关闭</button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import axios from "axios";
import MQTTService from "@/utils/MQTTService";
export default {
  data() {
    return {
      uid: 0,
      hanfuDetails: [],
      leaseOrders: {
        userId: "",
        hanfuId: "",
      },
      showModal: false,
      totalCost: 0,
      location: 0,
    };
  },
  created() {
    this.fetchOrders();
  },
  mounted() {
    const connectUrl = "ws://129.211.26.112:8083/mqtt";
    const options = {
      clean: true,
      connectTimeout: 4000,
      clientId: "ysc" + Math.random().toString(16).substr(2, 8),
      username: "ysc",
      password: "ysc",
    };

    this.mqttService = new MQTTService(connectUrl, options);
    this.mqttService.connect();
    this.mqttService.subscribe("control");

    this.mqttService.onMessage((topic, message) => {
      console.log(`收到消息: [${topic}] ${message}`);
    });

    // setTimeout(() => {
    //   this.mqttService.publish('test/topic', 'Hello MQTT!');
    // }, 2000);
  },

  beforeDestroy() {
    this.mqttService.disconnect();
  },
  methods: {
    async fetchOrders() {
      try {
        this.uid = this.$store.state.userInfo.userId; // 从 Vuex 中获取用户 ID
        const response = await axios.get(
          `/lease/myOutstandingOrders/${this.uid}`
        );
        const orders = response.data.data;
        this.getHanfuDetails(orders);
      } catch (error) {
        console.error("获取订单失败", error);
      }
    },
    async getHanfuDetails(orders) {
      try {
        this.hanfuDetails = await Promise.all(
          orders.map((order) =>
            axios.get(`/home/hanfu_detail/${order.hanfuId}`).then((res) => ({
              ...order,
              hanfu: res.data.data,
            }))
          )
        );
      } catch (error) {
        console.error("获取汉服详情失败", error);
      }
    },
    async confirmReturn(order) {
      const siteID = this.$route.params.siteID;

      console.log(siteID);
      this.leaseOrders.userId = this.$store.state.userInfo.userId; // 从 Vuex 获取用户 ID
      this.leaseOrders.hanfuId = order.hanfuId; // 当前汉服 ID

      try {
        // 发送归还请求，并获取归还地点和总花费
        const response = await axios.post(
          `/lease/CompleteOrder/${siteID}`,
          this.leaseOrders
        );

        if (response.data) {
          console.log(response);
          this.totalCost = response.data.data.totalCost; // 获取总花费
          this.location = response.data.data.location; // 获取归还地点

          const command = {
            action: "return",
            option: "open",
            cabinetLocation: this.location,
          };

          // 将对象序列化为 JSON 字符串后发送
          this.mqttService.publish("control", JSON.stringify(command));

          this.showModal = true; // 显示弹窗
        } else {
          window.alert("归还失败，请重试");
        }
      } catch (error) {
        console.error("归还订单失败", error);
        window.alert("提交归还订单时发生错误，请重试");
      }
    },
    closeModal() {
      const command = {
        action: "return",
        option: "close",
        cabinetLocation: this.location,
      };

      this.mqttService.publish("control", JSON.stringify(command));

      this.showModal = false; // 关闭弹窗

      setTimeout(() => {
        window.location.reload();
      }, 1000);
    },
  },
};
</script>

<style scoped>
.return-clothes {
  max-width: 800px;
  margin: 0 auto;
  padding: 30px;
  background-color: #fdfdfd;
  border-radius: 16px;
  box-shadow: 0 8px 16px rgba(0, 0, 0, 0.1);
  font-family: "KaiTi", serif;
}

.orders-container {
  display: flex;
  flex-direction: column;
  gap: 30px;
}

.order-card {
  background: rgba(255, 255, 255, 0.9);
  padding: 20px;
  border-radius: 16px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
  border: 1px solid #f1f1f1;
  transition: transform 0.3s, box-shadow 0.3s;
}

.order-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
}

h3 {
  font-size: 22px;
  color: #5a3e1b;
  margin-bottom: 15px;
}

.hanfu-details {
  margin-top: 15px;
}

.hanfu-info {
  display: flex;
  gap: 20px;
  align-items: center;
}

.hanfu-img-container {
  flex: 0 0 auto;
}

.hanfu-img {
  width: 200px;
  height: 300px;
  object-fit: cover;
  border-radius: 10px;
  transition: transform 0.3s;
}

.hanfu-img:hover {
  transform: scale(1.05);
}

.hanfu-text-info {
  flex: 1;
}

.hanfu-text-info p {
  margin: 8px 0;
  color: #5a3e1b;
  font-size: 16px;
}

button {
  width: 100%;
  padding: 14px;
  margin-top: 20px;
  background-color: #8b4513;
  color: white;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  font-size: 18px;
  transition: background-color 0.3s;
}

button:hover {
  background-color: #7a3f1f;
}

button:disabled {
  background-color: #ccc;
}

.message {
  margin-top: 15px;
  color: #6b4226;
  text-align: center;
}

.no-orders {
  text-align: center;
  color: #6b4226;
  margin-top: 30px;
  font-size: 18px;
}

/* 弹窗样式 */
/* 弹窗外部遮罩层 */
.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.6);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

/* 弹窗内容 */
.modal-content {
  background: linear-gradient(135deg, #ffecd2, #fcb69f); /* 渐变背景 */
  padding: 20px;
  border-radius: 15px;
  box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1);
  max-width: 400px;
  width: 100%;
  animation: fadeIn 0.3s ease, scaleUp 0.3s ease-out; /* 淡入和放大动画 */
}

/* 弹窗正文 */
.modal-body {
  font-size: 16px;
  color: #555;
  text-align: center;
}

/* 弹窗底部按钮 */
.modal-footer {
  display: flex;
  justify-content: center;
  padding-top: 10px;
}

.btn-close {
  background-color: #2980b9;
  color: white;
  border: none;
  padding: 12px 25px;
  border-radius: 50px;
  font-size: 18px;
  transition: background-color 0.3s ease;
}

.btn-close:hover {
  background-color: #3498db; /* 按钮悬停背景色 */
}

/* 弹窗淡入动画 */
@keyframes fadeIn {
  0% {
    opacity: 0;
  }
  100% {
    opacity: 1;
  }
}

/* 弹窗弹出动画 */
@keyframes scaleUp {
  0% {
    transform: scale(0.8);
    opacity: 0;
  }
  100% {
    transform: scale(1);
    opacity: 1;
  }
}

/* 弹窗头部装饰：花朵图案 */
.modal-header:before {
  content: "🌸"; /* 使用花朵emoji */
  font-size: 24px;
  margin-right: 10px;
  color: #ff4081;
}

/* 弹窗底部增加星星图案 */
.modal-footer:after {
  content: "⭐";
  font-size: 30px;
  color: #ffd700;
  position: absolute;
  bottom: -10px;
  right: 50%;
  transform: translateX(50%);
}
</style>
