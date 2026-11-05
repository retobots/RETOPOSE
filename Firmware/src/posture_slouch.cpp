#include "posture_slouch.h"
#include "config.h"
#include <math.h>

static uint32_t slouch_timer_ms = 0;

bool check_slouch(const IMU_Data& data, uint32_t dt_ms) {
    // 1. Kiểm tra góc gù lưng dựa vào biến config
    if (data.pitch > ANGLE_SLOUCH_THRESHOLD) {
        
        // 2. Chống báo động giả: So sánh với ngưỡng động (Dynamic Threshold)
        if (abs(data.gyro_x) > GYRO_DYNAMIC_THRESHOLD) {
            return false;
        }

        slouch_timer_ms += dt_ms;
        
        // 3. Nếu gù lưng liên tục quá thời gian quy định (5000ms)
        if (slouch_timer_ms >= TIME_SLOUCH_MS) {
            return true; // Kích hoạt rung
        }
    } else {
        // NGƯỜI DÙNG ĐÃ NGỒI THẲNG LẠI (Hoặc nhỏ hơn ngưỡng gù)
        slouch_timer_ms = 0; 
    }
    return false;
}

void slouch_timer_reset() {
    slouch_timer_ms = 0;
}