#ifndef CONFIG_H
#define CONFIG_H

//giao tiep I2C cho MPU 6050
#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9

//pinout cho motor
#define PIN_VIBRATOR 10
#define PIN_BUZZER 12
#define PIN_LED 2

//goc nghieng cho cac tinh huong
#define ANGLE_NORMAL_THRESHOLD 5.0f
#define ANGLE_SLOUCH_THRESHOLD 15.0f
#define ANGLE_STRETCH_THRESHOLD -10.0f
#define ANGLE_DROPPEN_THRESHOLD 45.0f

//thoi gian cho cac tinh huong
#define TIME_SLOUCH_MS 5000
#define TIME_DROP_PEN_MIN_MS 1000
#define TIME_DROP_PEN_MAX_MS 3000
#define TIME_STRETCH_MAX_MS 60000

#endif