#ifndef POSTURE_STRETCH_H
#define POSTURE_STRETCH_H
#include <stdint.h>
#include "config.h"
bool check_stretch(const IMU_Data &imu, uint32_t dt_ms);
void stretch_timer_reset();

#endif // POSTURE_STRETCH_H