<template>
  <div class="clothing-details">
    <!-- 服饰图集 -->
    <div class="image-list">
      <h3>服饰图集:</h3>
      <div class="image-thumbnails">
        <div
          v-for="(item, index) in clothingData.imageList"
          :key="index"
          class="thumbnail-container"
        >
          <img
            :src="item.image"
            alt="Image Thumbnail"
            class="thumbnail"
            @click="openImage(item.image)"
          />
        </div>
      </div>
    </div>

    <!-- 详情区域 -->
    <div class="details-container">
      <h1>{{ clothingData.name }}</h1>
      <div class="info-section">
        <p><strong>租金:</strong> ¥{{ clothingData.rent }} / 天</p>
        <p>
          <strong>描述:</strong> {{ clothingData.description || "暂无描述" }}
        </p>
        <button class="rent-button" @click="rentClothing()">租借</button>
      </div>
    </div>

    <!-- 提示框 -->
    <div v-if="isModalOpen" class="modal" @click.self="closeModal">
      <div class="modal-content">
        <p>{{ modalMessage }}</p>
        <button
          v-if="modalMessage !== '您还未登录，请先登录！'"
          class="close-button"
          @click="closeModal"
        >
          关闭
        </button>
      </div>
    </div>
  </div>
</template>

<script>
import axios from "axios";
import { mapState } from "vuex";
import MQTTService from "@/utils/MQTTService";

export default {
  name: "ClothingDetails",
  props: ["id"],
  data() {
    return {
      backendUrl: "http://192.168.137.36:5000/lease", // 请替换为实际的树莓派 IP
      isLoading: false,
      message: "",
      messageType: "info",
      clothingData: {},
      isModalOpen: false, // 控制提示框显示
      modalMessage: "", // 提示框内容

      leaseOrders: {
        userId: "",
        hanfuId: "",
      },
    };
  },
  computed: {
    ...mapState(["closestDeviceId"]),
    ...mapState(["userInfo"]),
  },
  mounted() {
    this.fetchClothingDetails();

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
    fetchClothingDetails() {
      const clothingID = this.id;
      axios
        .get(`/home/hanfu_detail/${clothingID}`)
        .then((response) => {
          this.clothingData = response.data.data;
        })
        .catch((error) => {
          console.error("Error fetching clothing details:", error);
        });
    },

    async rentClothing() {
      if (this.userInfo == null) {
        this.modalMessage = "您还未登录，请先登录！";
        this.isModalOpen = true;

        // 3秒后跳转到首页
        setTimeout(() => {
          this.$router.push("/");
        }, 3000);
      } else {
        let command = this.clothingData.cabinetLocation;
        this.isLoading = true;
        this.message = "";

        this.createLesaseOrders();

        try {
          // const response = await fetch(this.backendUrl, {
          //   method: 'POST',
          //   headers: {
          //     'Content-Type': 'application/json',
          //   },
          //   body: JSON.stringify({ command }),
          // });

          // if (!response.ok) {
          //   throw new Error('网络响应异常');
          // }

          // const data = await response.json();

          // if (data !== "无效命令") {
          // this.message = `操作成功: ${JSON.stringify(data)}`;
          // this.messageType = "info";
          // 弹出成功提示框
          this.modalMessage = `租借成功！请及时前往${this.clothingData.cabinetLocation}号柜门取走衣服！`;

          // 封装消息结构体
          const command = {
            topic: "control",
            msg: {
              action: "rent",
              option: "open",
              // 动作类型，比如打开柜门
              cabinetLocation: this.clothingData.cabinetLocation, // 衣柜位置
              clothesID:this.clothingData.hanFuId
            },
          };

          // 将对象序列化为 JSON 字符串后发送
          this.mqttService.publish("control", JSON.stringify(command));

          this.isModalOpen = true;
          // } else {
          //     this.modalMessage=`租借操作失败，请稍后再试！`;
          // }
        } catch (error) {
          this.message = `错误: ${error.message}`;
          this.messageType = "error";

          // 弹出失败提示框
          this.modalMessage = "租借操作失败，请稍后再试！";
          this.isModalOpen = true;
        } finally {
          this.isLoading = false;
        }
      }
    },

    async createLesaseOrders() {
      // 假设 leaseOrders 对象需要包含租借人 ID 和衣物的 ID，这里需要将它们填充。
      this.leaseOrders.userId = this.$store.state.userInfo.userId; // 假设从 Vuex 中获取用户 ID

      this.leaseOrders.hanfuId = this.clothingData.hanFuId; // 假设 clothingData 包含当前衣服的 ID
      // const siteID = this.closestDeviceId
      // const siteID= 13
      // console.log(this.clothingData)
      // console.log(this.leaseOrders.userId)
      // console.log(this.leaseOrders.hanfuId)
      try {
        const response = await axios.post(`/lease/order/13`, this.leaseOrders);

        if (response.data.msg !== "success") {
          throw new Error("网络响应异常");
        }
      } catch (error) {
        console.error("创建租借订单失败:", error);
        // 出现错误时，弹出提示框
        this.modalMessage = `错误: ${error.message}`;
        this.isModalOpen = true;
      }
    },

    closeModal() {
      this.isModalOpen = false;
      console.log(this.clothingData.hanFuId);

      const command = {
        topic: "control",
        msg: {
          action: "rent",
          option: "close",
          // 动作类型，比如打开柜门
          cabinetLocation: this.clothingData.cabinetLocation, // 衣柜位置
          clothesID:this.clothingData.hanFuId
        },
      };
      this.mqttService.publish("control", JSON.stringify(command));

      setTimeout(() => {
        this.$router.push({
          name: "WardrobeDetails",
          params: { siteID: 13 },
        });
      }, 1000);
    },
  },
};
</script>

<style scoped>
.clothing-details {
  padding: 50px;
  background-color: #f9f6f0;
  border-radius: 15px;
  text-align: center;
  box-shadow: 0 6px 15px rgba(0, 0, 0, 0.1);
  max-width: 1200px;
  margin: 0 auto;
  font-family: "Arial", sans-serif;
  overflow: hidden;
}

h1 {
  font-size: 36px;
  font-weight: bold;
  color: #5f4037;
  margin-bottom: 20px;
}

.image-list {
  margin-top: -40px;
  margin-bottom: 40px;
}

.image-thumbnails {
  display: flex;
  gap: 20px;
  justify-content: flex-start;
  flex-wrap: nowrap;
  overflow-x: auto;
  -webkit-overflow-scrolling: touch;
}

.thumbnail-container {
  flex-shrink: 0;
}

.thumbnail {
  width: 350px;
  height: 350px;
  object-fit: cover;
  border-radius: 8px;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
  transition: transform 0.3s ease;
  cursor: pointer;
}

.thumbnail:hover {
  transform: scale(1.1);
}

.details-container {
  margin-top: 40px;
  text-align: left;
  font-family: "KaiTi", serif;
}

.info-section p {
  margin: 12px 0;
  font-size: 18px;
  line-height: 1.5;
  color: #7b4b2b;
}

.rent-button {
  padding: 12px 30px;
  background-color: #8b5e3c;
  color: white;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  font-size: 18px;
  font-weight: bold;
  transition: background-color 0.3s ease, transform 0.2s ease;
  margin-top: 20px;
}

.rent-button:hover {
  background-color: #6a4326;
  transform: scale(1.05);
}

/* 提示框样式 */
/* 弹窗外部遮罩层 */
.modal {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  /* 半透明背景 */
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
  animation: fadeIn 0.5s ease-out;
  /* 弹窗进入的动画 */
}

/* 弹窗内容 */
.modal-content {
  background: #fff;
  padding: 20px 30px;
  border-radius: 15px;
  /* 圆角效果 */
  box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
  max-width: 400px;
  width: 100%;
  text-align: center;
  animation: slideIn 0.5s ease-out;
  /* 弹窗滑入的动画 */
}

/* 弹窗正文 */
.modal-content p {
  font-size: 18px;
  color: #333;
  margin-bottom: 20px;
  animation: fadeInText 0.5s ease-out;
}

/* 美化按钮 */
.close-button {
  background: linear-gradient(135deg, #2980b9, #3498db);
  /* 渐变色背景 */
  color: white;
  border: none;
  padding: 12px 25px;
  border-radius: 30px;
  font-size: 18px;
  cursor: pointer;
  transition: background-color 0.3s ease, transform 0.3s ease;
}

/* 按钮悬停时的效果 */
.close-button:hover {
  background: linear-gradient(135deg, #db6334, #b94829);
  /* 背景渐变色反转 */
  transform: scale(1.05);
  /* 放大效果 */
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

/* 弹窗滑入动画 */
@keyframes slideIn {
  0% {
    transform: translateY(-50px);
    opacity: 0;
  }

  100% {
    transform: translateY(0);
    opacity: 1;
  }
}

/* 弹窗文本淡入动画 */
@keyframes fadeInText {
  0% {
    opacity: 0;
  }

  100% {
    opacity: 1;
  }
}
</style>
