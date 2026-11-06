#ifndef POSTURE_WALK_H
#define POSTURE_WALK_H

#include <stdint.h>
#include "imu_sensor.h"
#include "config.h"

bool check_walk(const IMU_Data &imu, float delta_pitch, uint32_t dt_ms);
void walk_timer_reset();

#endif
