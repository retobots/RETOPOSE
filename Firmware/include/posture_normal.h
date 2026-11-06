#include "imu_sensor.h"
#ifndef POSTURE_NORMAL_H
#define POSTURE_NORMAL_H
#include <stdint.h>
#include "config.h"
bool check_normal(float m_delta_pitch, const IMU_Data &imu);

#endif