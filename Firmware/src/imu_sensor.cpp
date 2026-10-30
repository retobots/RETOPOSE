#include "imu_sensor.h"
#include "config.h"
#include <Wire.h>
#include <math.h>

// Địa chỉ I2C mặc định của MPU6050
constexpr uint8_t MPU6050_ADDR = 0x68;

bool imu_init() {
    // Khởi tạo I2C với 2 chân SDA và SCL được định nghĩa trong config.h
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    
    // Đánh thức MPU6050 (mặc định nó ở chế độ Sleep)
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B); // Thanh ghi quản lý nguồn (PWR_MGMT_1)
    Wire.write(0x00); // Ghi giá trị 0 để đánh thức
    uint8_t error = Wire.endTransmission();
    
    return (error == 0); // Trả về true nếu giao tiếp I2C thành công
}

bool imu_read_data(IMU_Data &data) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B); // Bắt đầu đọc từ thanh ghi gia tốc trục X (ACCEL_XOUT_H)
    if (Wire.endTransmission(false) != 0) {
        return false; // Lỗi kết nối
    }
    
    // Yêu cầu đọc 14 byte (6 byte Accel, 2 byte Temp, 6 byte Gyro)
    Wire.requestFrom((uint16_t)MPU6050_ADDR, (uint8_t)14, (uint8_t)true);
    if (Wire.available() < 14) {
        return false;
    }
    
    // Đọc gia tốc thô (Raw Data)
    int16_t acc_x = (Wire.read() << 8 | Wire.read());
    int16_t acc_y = (Wire.read() << 8 | Wire.read());
    int16_t acc_z = (Wire.read() << 8 | Wire.read());
    
    // Bỏ qua 2 byte nhiệt độ
    Wire.read(); 
    Wire.read();
    
    // Đọc vận tốc góc thô (Raw Data)
    int16_t gyro_x = (Wire.read() << 8 | Wire.read());
    int16_t gyro_y = (Wire.read() << 8 | Wire.read());
    int16_t gyro_z_raw = (Wire.read() << 8 | Wire.read());
    
    // Chuyển đổi sang đơn vị chuẩn (Giả sử LSB/g = 16384 và LSB/deg/s = 131)
    float ax = acc_x / 16384.0f;
    float ay = acc_y / 16384.0f;
    float az = acc_z / 16384.0f;
    data.gyro_z = gyro_z_raw / 131.0f;
    
    // Tính toán gia tốc tổng hợp (dùng định lý Pytago không gian)
    data.accel_total = sqrt(ax*ax + ay*ay + az*az);
    
    // Tính toán góc nghiêng Pitch và Roll dựa trên gia tốc trọng trường
    data.pitch = atan2(-ax, sqrt(ay*ay + az*az)) * 180.0f / PI;
    data.roll  = atan2(ay, az) * 180.0f / PI;
    
    return true;
}
void imu_enable_wake_on_motion() {
    // 1. Cài đặt ngưỡng lực chuyển động (Motion Threshold)
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x1F); // Thanh ghi MOT_THR
    Wire.write(20);   // Giá trị ngưỡng (có thể tinh chỉnh 10-40)
    Wire.endTransmission();

    // 2. Cài đặt thời gian duy trì chuyển động (Motion Duration)
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x20); // Thanh ghi MOT_DUR
    Wire.write(1);    // 1 mili-giây
    Wire.endTransmission();

    // 3. Kích hoạt tính năng ngắt chuyển động ra chân INT
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x38); // Thanh ghi INT_ENABLE
    Wire.write(0x40); // Bật bit số 6 (MOT_INT_EN)
    Wire.endTransmission();
}