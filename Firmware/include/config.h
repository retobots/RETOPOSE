#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Giao tiếp I2C cho MPU 6050
constexpr uint8_t PIN_I2C_SDA = 8;
constexpr uint8_t PIN_I2C_SCL = 9;
constexpr uint8_t PIN_IMU_INT = 5; // Chân nhận ngắt đánh thức

// Pinout cho ngoại vi
constexpr uint8_t PIN_VIBRATOR = 10;
constexpr uint8_t PIN_BUZZER   = 3;
constexpr uint8_t PIN_LED      = 2;
constexpr uint8_t PIN_BUTTON_RESET = 4; // Đã sửa đúng tên biến dùng trong analyzer

// Góc nghiêng cho các tình huống (Bắt buộc dùng float)
constexpr float ANGLE_NORMAL_THRESHOLD  = 5.0f;
constexpr float ANGLE_SLOUCH_THRESHOLD  = 15.0f;
constexpr float ANGLE_STRETCH_THRESHOLD = -10.0f;
constexpr float ANGLE_DROPPEN_THRESHOLD = 45.0f;

// Thời gian cho các tình huống (Bắt buộc dùng uint32_t)
constexpr uint32_t TIME_SLOUCH_MS       = 5000;
constexpr uint32_t TIME_DROP_PEN_MIN_MS = 1000;
constexpr uint32_t TIME_DROP_PEN_MAX_MS = 3000;
constexpr uint32_t TIME_STRETCH_MAX_MS  = 60000;

// Ngưỡng con quay hồi chuyển phân biệt Động/Tĩnh (Độ/giây)
constexpr float GYRO_DYNAMIC_THRESHOLD = 35.0f;

// Thông số cho chế độ ngủ sâu (Deep Sleep)
constexpr float ACCEL_STATIC_THRESHOLD = 0.15f; 
constexpr float GYRO_STATIC_THRESHOLD  = 10.0f; 
constexpr uint32_t TIME_IDLE_DEEP_SLEEP_MS = 180000; 
// Ngưỡng gia tốc nhận diện đi bộ/đứng dậy (g)
constexpr float WALK_ACCEL_THRESHOLD = 1.2f;

#endif