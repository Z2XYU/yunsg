const { defineConfig } = require('@vue/cli-service')
const webpack = require('webpack')

module.exports = defineConfig({
  transpileDependencies: true,

  configureWebpack: {
    resolve: {
      fallback: {
        url: require.resolve('url/'),                   // 添加 polyfill
        stream: require.resolve('stream-browserify'),   // 添加 polyfill
        crypto: require.resolve('crypto-browserify'),   // 可选，如果遇到 crypto 错误
        buffer: require.resolve('buffer/'),             // 可选，如果遇到 buffer 错误
        process: require.resolve('process/browser')     // 可选，处理 process
      }
    },
    plugins: [
      new webpack.ProvidePlugin({
        process: 'process/browser',
        Buffer: ['buffer', 'Buffer']
      })
    ]
  }
})
