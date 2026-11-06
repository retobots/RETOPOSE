#include "posture_stretch.h"
#include "config.h"

static uint32_t stretch_timer_ms = 0;
static bool stretch_timed_out = false;

bool check_stretch(float m_delta_pitch, const IMU_Data &imu, uint32_t dt_ms) {
    // Vươn vai / ngả lưng thư giãn: pitch âm vượt ngưỡng -10 độ
    if (m_delta_pitch <= ANGLE_STRETCH_THRESHOLD) {
        stretch_timer_ms += dt_ms;

        // Cho phép thư giãn trong giới hạn thời gian (<= 60 giây)
        if (stretch_timer_ms <= TIME_STRETCH_MAX_MS) {
            stretch_timed_out = false;
            return true;
        } else {
            // Quá 60 giây: ngủ gật gục ghế -> kích hoạt cờ cảnh báo
            stretch_timed_out = true;
            return false;
        }
    } else {
        stretch_timer_reset();
    }

    return false;
}

bool is_stretch_timeout() {
    return stretch_timed_out;
}

void stretch_timer_reset() {
    stretch_timer_ms = 0;
    stretch_timed_out = false;
}

uint32_t get_stretch_timer_ms() {
    return stretch_timer_ms;
}
