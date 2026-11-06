#include "imu_sensor.h"
#ifndef POSTURE_STRETCH_H
#define POSTURE_STRETCH_H
#include <stdint.h>
#include "config.h"
bool check_stretch(float m_delta_pitch, const IMU_Data &imu, uint32_t dt_ms);
void stretch_timer_reset();

#endif // POSTURE_STRETCH_H