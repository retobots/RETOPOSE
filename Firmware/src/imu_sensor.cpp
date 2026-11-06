#include "imu_sensor.h"
#include "config.h"
#include <Wire.h>
#include <math.h>

constexpr uint8_t MPU6050_ADDR = 0x68;

static float gyro_offset_x = 0.0f;
static float gyro_offset_y = 0.0f;
static float gyro_offset_z = 0.0f;

bool imu_init() {
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
    Wire.setClock(400000); // 400kHz Fast I2C

    // Đánh thức MPU6050
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x6B); // PWR_MGMT_1
    Wire.write(0x00); // Ghi 0 để thoát Sleep Mode
    uint8_t error = Wire.endTransmission();
    if (error != 0) {
        return false;
    }

    delay(30);

    // Cấu hình Gyro full scale range +/- 250 deg/s (131 LSB/deg/s)
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x1B); // GYRO_CONFIG
    Wire.write(0x00);
    Wire.endTransmission();

    // Cấu hình Accel full scale range +/- 2g (16384 LSB/g)
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x1C); // ACCEL_CONFIG
    Wire.write(0x00);
    Wire.endTransmission();

    // Hiệu chuẩn bù trôi tĩnh Gyroscope (Zero-Rate Offset Calibration)
    float gx_sum = 0.0f, gy_sum = 0.0f, gz_sum = 0.0f;
    int samples = 50;
    int valid = 0;
    for (int i = 0; i < samples; i++) {
        Wire.beginTransmission(MPU6050_ADDR);
        Wire.write(0x43); // GYRO_XOUT_H
        if (Wire.endTransmission(false) == 0 && Wire.requestFrom((uint16_t)MPU6050_ADDR, (uint8_t)6, (uint8_t)true) >= 6) {
            int16_t gx = (Wire.read() << 8 | Wire.read());
            int16_t gy = (Wire.read() << 8 | Wire.read());
            int16_t gz = (Wire.read() << 8 | Wire.read());
            gx_sum += gx / 131.0f;
            gy_sum += gy / 131.0f;
            gz_sum += gz / 131.0f;
            valid++;
        }
        delay(4);
    }
    if (valid > 0) {
        gyro_offset_x = gx_sum / valid;
        gyro_offset_y = gy_sum / valid;
        gyro_offset_z = gz_sum / valid;
    }

    return true;
}

bool imu_read_data(IMU_Data &data) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B); // Bắt đầu đọc từ ACCEL_XOUT_H
    if (Wire.endTransmission(false) != 0) {
        return false;
    }

    // Đọc 14 byte (Accel 6 byte, Temp 2 byte, Gyro 6 byte)
    if (Wire.requestFrom((uint16_t)MPU6050_ADDR, (uint8_t)14, (uint8_t)true) < 14) {
        return false;
    }

    int16_t acc_x = (Wire.read() << 8 | Wire.read());
    int16_t acc_y = (Wire.read() << 8 | Wire.read());
    int16_t acc_z = (Wire.read() << 8 | Wire.read());

    // Bỏ qua 2 byte nhiệt độ
    Wire.read();
    Wire.read();

    int16_t raw_gyro_x = (Wire.read() << 8 | Wire.read());
    int16_t raw_gyro_y = (Wire.read() << 8 | Wire.read());
    int16_t raw_gyro_z = (Wire.read() << 8 | Wire.read());

    float ax = acc_x / 16384.0f;
    float ay = acc_y / 16384.0f;
    float az = acc_z / 16384.0f;

    // Trừ bù độ lệch tĩnh con quay hồi chuyển
    data.gyro_x = (raw_gyro_x / 131.0f) - gyro_offset_x;
    data.gyro_y = (raw_gyro_y / 131.0f) - gyro_offset_y;
    data.gyro_z = (raw_gyro_z / 131.0f) - gyro_offset_z;

    data.accel_total = sqrt(ax * ax + ay * ay + az * az);

    // Tính toán góc theo gia tốc kế (Y hướng lên đầu, Z đâm vào lưng)
    float accel_pitch = atan2(-az, sqrt(ax * ax + ay * ay)) * 180.0f / PI;
    float accel_roll  = atan2(ax, ay) * 180.0f / PI;

    float gyro_pitch_rate = data.gyro_x;
    float gyro_roll_rate  = data.gyro_z;

    static uint32_t last_time_micros = 0;
    static float current_pitch = 0.0f;
    static float current_roll  = 0.0f;
    static bool is_first_run   = true;

    uint32_t current_time_micros = micros();

    // Khởi tạo tức thì góc đo ở chu kỳ đầu tiên
    if (is_first_run) {
        is_first_run = false;
        last_time_micros = current_time_micros;
        current_pitch = accel_pitch;
        current_roll  = accel_roll;
        data.pitch = current_pitch;
        data.roll  = current_roll;
        return true;
    }

    float dt = (current_time_micros - last_time_micros) / 1000000.0f;
    last_time_micros = current_time_micros;

    // Bộ lọc bù (Complementary Filter)
    if (dt > 0.0f && dt < 0.2f) {
        current_pitch = 0.98f * (current_pitch + gyro_pitch_rate * dt) + 0.02f * accel_pitch;
        current_roll  = 0.98f * (current_roll  + gyro_roll_rate * dt)  + 0.02f * accel_roll;
    } else {
        // Đồng bộ nhanh nếu dt gián đoạn
        current_pitch = accel_pitch;
        current_roll  = accel_roll;
    }

    data.pitch = current_pitch;
    data.roll  = current_roll;

    return true;
}

void imu_enable_wake_on_motion() {
    // 1. Cài đặt ngưỡng lực chuyển động
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x1F); // MOT_THR
    Wire.write(20);
    Wire.endTransmission();

    // 2. Cài đặt thời gian duy trì
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x20); // MOT_DUR
    Wire.write(1);
    Wire.endTransmission();

    // 3. Kích hoạt ngắt ra chân INT
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x38); // INT_ENABLE
    Wire.write(0x40); // Bit 6: MOT_INT_EN
    Wire.endTransmission();
}
