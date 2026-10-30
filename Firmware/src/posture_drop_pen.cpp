#include "posture_drop_pen.h"
#include "config.h"

static uint32_t drop_pen_timer_ms = 0;

bool check_drop_pen(const IMU_Data &imu, uint32_t dt_ms) {
    // Nếu cúi gập người quá 45 độ
    if (imu.pitch >= ANGLE_DROPPEN_THRESHOLD) {
        drop_pen_timer_ms += dt_ms;
        
        // Thời gian duy trì từ 1s đến 3s thì tính là nhặt bút hợp lệ
        if (drop_pen_timer_ms >= TIME_DROP_PEN_MIN_MS && drop_pen_timer_ms <= TIME_DROP_PEN_MAX_MS) {
            return true;
        }
    } else {
        // Vừa ngồi thẳng dậy -> reset thời gian
        drop_pen_timer_reset();
    }
    return false;
}

void drop_pen_timer_reset() {
    drop_pen_timer_ms = 0;
}