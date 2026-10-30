#ifndef POSTURE_SLOUCH_H
#define POSTURE_SLOUCH_H
#include <stdint.h>
#include "config.h"
bool check_slouch(const IMU_Data &imu, uint32_t dt_ms);
void slouch_timer_reset();
#endif