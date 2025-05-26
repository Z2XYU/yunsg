<template>
  <div id="app">
    <div class="background-image"></div>
    <LoginForm />
  </div>
</template>

<script>
import LoginForm from './components/LoginForm.vue';

export default {
  name: 'App',
  components: {
    LoginForm
  }
}
</script>

<style>
#app {
  position: relative;
  height: 100vh;
  display: flex;
  justify-content: center;
  align-items: center;
}

.background-image {
  position: absolute;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-image: url('@/assets/images/background.jpg');
  background-size: cover;
  background-position: center;
  filter: blur(5px);
  z-index: -1;
}
</style>
