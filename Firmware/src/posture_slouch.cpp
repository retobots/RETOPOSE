#include "posture_slouch.h"
#include "config.h"
#include <math.h>

static uint32_t slouch_timer_ms = 0;

bool check_slouch(const IMU_Data &imu, uint32_t dt_ms) {
    if (imu.pitch >= ANGLE_SLOUCH_THRESHOLD) {
        
        // Dùng fabsf() dành riêng cho kiểu float
        if (fabsf(imu.gyro_z) < GYRO_DYNAMIC_THRESHOLD) {
            slouch_timer_ms += dt_ms; 
        }

        if (slouch_timer_ms >= TIME_SLOUCH_MS) {
            return true;
        }
    } else {
        slouch_timer_reset();
    }
    
    return false;
}

void slouch_timer_reset() {
    slouch_timer_ms = 0;
}