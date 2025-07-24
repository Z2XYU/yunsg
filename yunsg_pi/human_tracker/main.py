import cv2
import numpy as np
from Servo import ServoController
import time
from http.server import BaseHTTPRequestHandler, HTTPServer
import threading

# 创建 ServoController 实例，使用 PCA9685 控制舵机
servo_controller = ServoController(servo_x_channel=0, servo_y_channel=1)

cap = cv2.VideoCapture(0)
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_alt2.xml')

# 追踪状态变量
count = 0

# 初始的舵机角度，避免误动
x_angle, y_angle = 90, 100
last_x_angle, last_y_angle = 90, 100  # 用于保存上一次的舵机角度
max_angle_change = 5  # 每次最大角度变化，避免剧烈调整

# HTTP 服务器处理类
class VideoStreamHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/video':
            self.send_response(200)
            self.send_header('Content-type', 'multipart/x-mixed-replace; boundary=frame')
            self.end_headers()

            while True:
                ret, frame = cap.read()
                if not ret:
                    break

                # 每一帧未检测到人脸时增加计数
                global count, x_angle, y_angle, last_x_angle, last_y_angle

                # 转换为灰度图像
                gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

                # 检测人脸
                faces = face_cascade.detectMultiScale(gray, scaleFactor=1.5, minNeighbors=5)

                if len(faces) > 0:
                    count = 0  # 检测到人脸时，重置计数

                    x, y, w, h = faces[0]
                    print(x, y, w, h)

                    # 绘制人脸框
                    cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)

                    # 获取人脸中心位置
                    face_center_x = x + w // 2
                    face_center_y = y + h // 2

                    # 绘制人脸中心
                    cv2.circle(frame, (face_center_x, face_center_y), 5, (0, 255, 0), -1)

                    # 获取图像中心位置
                    frame_center_x = frame.shape[1] // 2
                    frame_center_y = frame.shape[0] // 2

                    # 计算人脸相对于图像中心的偏移量
                    delta_x = face_center_x - frame_center_x
                    delta_y = face_center_y - frame_center_y

                    # 将偏移量映射到舵机角度范围 (0-180)
                    new_x_angle = 90 - delta_x / frame.shape[1] * 180  # 将偏移映射到舵机角度范围
                    new_y_angle = 90 + delta_y / frame.shape[0] * 180  # 负号使得上移时舵机向上转

                    # 限制舵机角度在 0 到 180 度之间
                    new_x_angle = max(0, min(180, new_x_angle))
                    new_y_angle = max(0, min(180, new_y_angle))

                    # 只有当角度变化明显时才更新舵机
                    x_angle = new_x_angle
                    y_angle = new_y_angle

                    # 控制舵机，限制角度每次变化量最大为 max_angle_change，避免剧烈跳动
                    if abs(x_angle - last_x_angle) > max_angle_change:
                        x_angle = last_x_angle + np.sign(x_angle - last_x_angle) * max_angle_change

                    if abs(y_angle - last_y_angle) > max_angle_change:
                        y_angle = last_y_angle + np.sign(y_angle - last_y_angle) * max_angle_change

                    # 更新舵机位置
                    servo_controller.move_to_position(x_angle, y_angle)

                    # 保存当前角度为下一次计算提供参考
                    last_x_angle = x_angle
                    last_y_angle = y_angle
                else:
                    # 如果没有检测到人脸，检查 count 是否超过100
                    if count > 100:
                        # 超过阈值，恢复舵机到初始位置 (90, 90)
                        count = 0

                        servo_controller.move_to_position(90, 100)

                        # 延时来控制恢复速度
                        time.sleep(0.05)

                        print("No face detected. Returning to 90, 100.")

                # 将每一帧作为 MJPEG 数据推送
                ret, jpeg = cv2.imencode('.jpg', frame)
                if ret:
                    self.wfile.write(b'--frame\r\n')
                    self.send_header('Content-type', 'image/jpeg')
                    self.send_header('Content-length', str(len(jpeg)))
                    self.end_headers()
                    self.wfile.write(jpeg.tobytes())

                time.sleep(0.05)  # 控制帧率

# 启动 HTTP 服务器
def run_http_server():
    server_address = ('', 8080)
    httpd = HTTPServer(server_address, VideoStreamHandler)
    print("HTTP server running on port 8080...")
    httpd.serve_forever()

# 启动 HTTP 服务器的线程
http_server_thread = threading.Thread(target=run_http_server)
http_server_thread.daemon = True
http_server_thread.start()

# 持续获取视频流并提供 MJPEG 流
while True:
    time.sleep(1)

# 释放资源
servo_controller.cleanup()
cap.release()
cv2.destroyAllWindows()
