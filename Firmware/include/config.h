#ifndef CONFIG_H
#define CONGIF_H

//giao tiep I2C cho MPU 6050
#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9

//pinout cho motor
#define PIN_VIBRATOR 10

//goc nghien cho cac tinh huong
#define ANGLE_NORMAL_THRESHOLD 5.0f
#define ANGLE_SLOUCH_THRESHOLD 15.0f
#define ANGLE_STRETCH_THRESHOLD 10.0f
#define ANGLE_DROPPEN THRESHOLD 45.0f