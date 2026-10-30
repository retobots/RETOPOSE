#include "posture_stretch.h"
#include "config.h"

// Biến nội bộ đếm thời gian vươn vai
static uint32_t stretch_timer_ms = 0;

bool check_stretch(const IMU_Data &imu, uint32_t dt_ms) {
    // Vươn vai là ngả lưng về sau (pitch âm) vượt quá ngưỡng -10 độ
    if (imu.pitch <= ANGLE_STRETCH_THRESHOLD) {
        stretch_timer_ms += dt_ms;
        
        // Trả về true nếu đang vươn vai thư giãn hợp lệ (chưa vượt quá giới hạn 60 giây)
        // Nếu quá 60 giây (ngủ gật gục ghế), sẽ trả về false để hệ thống xử lý cảnh báo
        if (stretch_timer_ms <= TIME_STRETCH_MAX_MS) {
            return true;
        }
    } else {
        // Hết ngả lưng thì reset ngay bộ đếm
        stretch_timer_reset();
    }
    
    return false;
}

void stretch_timer_reset() {
    stretch_timer_ms = 0;
}