<template>
  <div class="wardrobe-details">
    <h1>{{ wardrobeData.name }}</h1>
    <p>{{ wardrobeData.description }}</p>

    <div v-if="wardrobeData.data && wardrobeData.data.length > 0" class="clothing-list">
      <div v-for="item in wardrobeData.data" :key="item.hanFuId" class="clothing-item"
        @click="viewClothingDetails(item.hanFuId)">
        <img :src="item.image" alt="Clothing Image" class="clothing-image" />
        <h3>{{ item.name }}</h3>
        <p class="price">租金: ¥{{ item.rent }} / 天</p>
      </div>
    </div>
    <div v-else>
      <p>暂无衣物</p>
    </div>
  </div>
</template>

<script>
import axios from "axios";
import { mapState } from 'vuex';

export default {
  name: "WardrobeDetails",
  data() {
    return {
      wardrobeData: {}, // 用于存储共享衣柜数据
      userInfo:{},
    };
  },
  computed:{
    ...mapState(['closestDeviceId']),
  },
  mounted() {
    const storedUserInfo = localStorage.getItem('userInfo');
    if (storedUserInfo) {
      this.userInfo = JSON.parse(storedUserInfo); // 解析 JSON 字符串
    }
    this.fetchWardrobeDetails();
  },
  methods: {
    fetchWardrobeDetails() {
      const siteID = this.$route.params.siteID
      axios
        .get(`/site/terminal/hanfu/${siteID}`)
        .then((response) => {
          this.wardrobeData = response.data;
          console.log("Wardrobe Details:", this.wardrobeData);
        })
        .catch((error) => {
          console.error("Error fetching wardrobe details:", error);
        });
    },
    viewClothingDetails(clothingId) {
      if (clothingId) {
        // 确保 clothingId 被正确传递
        this.$router.push({ name: "ClothingDetails", params: { id: clothingId } });
      } else {
        console.error("Clothing ID is undefined");
      }
    },
  },
};
</script>

<style scoped>
.wardrobe-details {
  padding: 30px;
  text-align: center;
  background-color: #f5f1e9;
  /* 轻柔的米色背景 */
  border-radius: 20px;
  box-shadow: 0 10px 20px rgba(0, 0, 0, 0.1);
  max-width: 1200px;
  margin: 0 auto;
  font-family: 'Arial', sans-serif;
}

h1 {
  font-size: 36px;
  font-weight: bold;
  color: #5f4037;
  /* 深棕色 */
  margin-bottom: 20px;
}

p {
  font-size: 18px;
  color: #7b4b2b;
  /* 古风棕色 */
  margin-bottom: 40px;
}

.clothing-list {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  /* 每行显示4件衣服，适应大屏显示 */
  gap: 30px;
  margin-top: 20px;
}

.clothing-item {
  background-color: #fff;
  padding: 20px;
  border-radius: 15px;
  text-align: center;
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
  transition: transform 0.3s ease-in-out, box-shadow 0.3s ease;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: space-between;
  height: 350px;
  cursor: pointer;
}

.clothing-item:hover {
  transform: scale(1.05);
  box-shadow: 0 8px 20px rgba(0, 0, 0, 0.2);
  /* 悬停时的阴影效果 */
}

.clothing-image {
  width: 100%;
  max-width: 200px;
  height: 250px;
  object-fit: cover;
  border-radius: 12px;
  /* 圆角图片 */
  margin-bottom: 15px;
}

.clothing-item h3 {
  font-size: 20px;
  margin: 15px 0;
  font-family: "KaiTi", serif;
  color: #8b5e3c;
  /* 深棕色 */
}

.clothing-item .price {
  font-size: 18px;
  font-family: "KaiTi", serif;
  color: #7b4b2b;
  /* 价格颜色 */
  margin: 10px 0;
}

@media (max-width: 1024px) {
  .clothing-list {
    grid-template-columns: repeat(3, 1fr);
    /* 中屏显示3列 */
  }
}

@media (max-width: 768px) {
  .clothing-list {
    grid-template-columns: repeat(2, 1fr);
    /* 小屏显示2列 */
  }

  .clothing-image {
    max-width: 180px;
    height: 220px;
  }

  h1 {
    font-size: 28px;
  }
}
</style>