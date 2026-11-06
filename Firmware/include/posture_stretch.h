#ifndef POSTURE_STRETCH_H
#define POSTURE_STRETCH_H

#include <stdint.h>
#include "imu_sensor.h"
#include "config.h"

bool check_stretch(float m_delta_pitch, const IMU_Data &imu, uint32_t dt_ms);
bool is_stretch_timeout();
void stretch_timer_reset();
uint32_t get_stretch_timer_ms();

#endif // POSTURE_STRETCH_H
