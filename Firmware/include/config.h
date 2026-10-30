#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

//giao tiep I2C cho MPU 6050
constexpr uint8_t PIN_I2C_SDA = 8;
constexpr uint8_t PIN_I2C_SCL  = 9;

//pinout cho motor
constexpr uint8_t PIN_VIBRATOR = 10;
constexpr uint8_t PIN_BUZZER = 3;
constexpr uint8_t PIN_LED = 2;
constexpr uint8_t PIN_BUTTON = 4;

//goc nghieng cho cac tinh huong
constexpr uint8_t ANGLE_NORMAL_THRESHOLD = 5.0f;
constexpr uint8_t ANGLE_SLOUCH_THRESHOLD = 15.0f;
constexpr uint8_t ANGLE_STRETCH_THRESHOLD = -10.0f;
constexpr uint8_t ANGLE_DROPPEN_THRESHOLD = 45.0f;

//thoi gian cho cac tinh huong
constexpr uint8_t TIME_SLOUCH_MS = 5000;
constexpr uint8_t TIME_DROP_PEN_MIN_MS = 1000;
constexpr uint8_t TIME_DROP_PEN_MAX_MS = 3000;
constexpr uint8_t TIME_STRETCH_MAX_MS = 60000;

#endif