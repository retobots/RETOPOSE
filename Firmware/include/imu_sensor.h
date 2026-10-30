#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#include <Arduino.h>

struct IMU_Data{
    float pitch = 0.0f; //nghieng truoc sau 
    float roll = 0.0f; //nghieng trai phai
    float gyro_z = 0.0f; //vtoc goc
    float accel_total = 0.0f;
};

bool imu_init();
bool imu_read_data(IMU_Data &data);
// Hàm cấu hình MPU6050 phát tín hiệu báo động khi có chuyển động
void imu_enable_wake_on_motion();

#endif