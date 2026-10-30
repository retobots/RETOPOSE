#include "posture_slouch.h"
#include "config.h"

// Biến nội bộ lưu trữ thời gian gù lưng cộng dồn
static uint32_t slouch_timer_ms = 0;

bool check_slouch(const IMU_Data &imu, uint32_t dt_ms) {
    // Kiểm tra góc pitch (nghiêng tới trước) có vượt ngưỡng gù lưng không
    if (imu.pitch >= ANGLE_SLOUCH_THRESHOLD) {
        slouch_timer_ms += dt_ms; // Cộng dồn thời gian vòng lặp
        
        // Nếu giữ tư thế gù lưng vượt quá thời gian cho phép (5000ms)
        if (slouch_timer_ms >= TIME_SLOUCH_MS) {
            return true; // Yêu cầu nhạc trưởng bật rung
        }
    } else {
        // Nếu người dùng thẳng lưng lại, reset ngay bộ đếm thời gian
        slouch_timer_reset();
    }
    
    return false; // Chưa tới ngưỡng hoặc đã thẳng lưng
}

void slouch_timer_reset() {
    slouch_timer_ms = 0;
}