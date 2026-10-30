#include "posture_drop_pen.h"
#include "config.h"
#include <math.h>

static uint32_t drop_pen_timer_ms = 0;
static bool is_dynamic_movement = false; // Cờ ghi nhận có chuyển động nhanh

bool check_drop_pen(const IMU_Data &imu, uint32_t dt_ms) {
    if (imu.pitch >= ANGLE_DROPPEN_THRESHOLD) {
        drop_pen_timer_ms += dt_ms;
        
        // Ghi nhận ngay nếu có khoảnh khắc gập người cực nhanh (FR-05)
        if (abs(imu.gyro_z) >= GYRO_DYNAMIC_THRESHOLD) {
            is_dynamic_movement = true;
        }
        
        // Nhặt bút hợp lệ = Thời gian ngắn VÀ có tính chất "chuyển động động"
        if (drop_pen_timer_ms >= TIME_DROP_PEN_MIN_MS && 
            drop_pen_timer_ms <= TIME_DROP_PEN_MAX_MS && 
            is_dynamic_movement) {
            return true;
        }
    } else {
        drop_pen_timer_reset();
    }
    return false;
}

void drop_pen_timer_reset() {
    drop_pen_timer_ms = 0;
    is_dynamic_movement = false; // Reset cờ
}