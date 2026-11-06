#ifndef POSTURE_DROP_PEN_H
#define POSTURE_DROP_PEN_H

#include <stdint.h>
#include "imu_sensor.h"
#include "config.h"

bool check_drop_pen(float m_delta_pitch, const IMU_Data &imu, uint32_t dt_ms);
void drop_pen_timer_reset();
uint32_t get_drop_pen_timer_ms();

#endif
