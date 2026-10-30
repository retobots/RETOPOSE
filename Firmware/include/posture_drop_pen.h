#ifndef POSTURE_DROP_PEN_H //ktra có  định nghĩa POSTURE_DROP_PEN_H chưa
#define POSTURE_DROP_PEN_H //chưa thì sẽ định nghĩa POSTURE_DROP_PEN_H
#include <stdint.h>
#include "config.h"
bool check_drop_pen(const IMU_Data &imu, uint32_t dt_ms);
void drop_pen_timer_reset();
#endif