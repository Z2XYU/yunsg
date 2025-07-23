import time
from adafruit_pca9685 import PCA9685
from adafruit_motor import servo
import board
import busio


class ServoController:
    def __init__(self, servo_x_channel, servo_y_channel):
        # 创建 I2C 接口
        i2c = busio.I2C(board.SCL,board.SDA)
        
        # 创建 PCA9685 对象
        self.pca = PCA9685(i2c)
        self.pca.frequency = 50  # 设置 PWM 频率为 50Hz
        
        # 存储舵机的通道号
        self.servo_x_channel = servo_x_channel
        self.servo_y_channel = servo_y_channel
        
        # 创建 Servo 实例，连接到 PCA9685 的通道
        self.servo_x = servo.Servo(self.pca.channels[servo_x_channel])
        self.servo_y = servo.Servo(self.pca.channels[servo_y_channel])
        
        # 初始化舵机位置（默认是 90 度）
        self.move_to_position(90, 100)

    def move_to_position(self, x_angle, y_angle):
        """
        控制舵机移动到指定位置
        :param x_angle: 水平舵机的角度
        :param y_angle: 垂直舵机的角度
        """
        # 将角度设置到舵机
        self.servo_x.angle = x_angle
        self.servo_y.angle = y_angle

    def cleanup(self):
        """清理资源"""
        self.pca.deinit()  # 停止 PCA9685 并释放资源

