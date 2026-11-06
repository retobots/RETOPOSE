#include "posture_slouch.h"
#include "config.h"
#include <math.h>

static uint32_t slouch_timer_ms = 0;
static bool is_slouching_state = false;

bool check_slouch(float m_delta_pitch, const IMU_Data& data, uint32_t dt_ms) {
    // Vào trạng thái nghi ngờ gù khi lệch quá ngưỡng
    if (m_delta_pitch > ANGLE_SLOUCH_THRESHOLD) {
        is_slouching_state = true;
    } else if (m_delta_pitch < (ANGLE_SLOUCH_THRESHOLD - 3.0f)) {
        // Hysteresis: Thoát trạng thái gù và reset timer khi đã thẳng lưng
        is_slouching_state = false;
        slouch_timer_reset();
        return false;
    }

    if (is_slouching_state) {
        // Chỉ cộng dồn thời gian khi người dùng tĩnh lặng (FR-05)
        // Nếu vặn người mạnh, tạm dừng đếm để chống báo động giả
        if (fabs(data.gyro_x) <= GYRO_DYNAMIC_THRESHOLD) {
            slouch_timer_ms += dt_ms;
        }

        // Kích hoạt báo động khi duy trì tư thế gù liên tục đủ thời gian (mặc định > 5s)
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
    is_slouching_state = false;
}

uint32_t get_slouch_timer_ms() {
    return slouch_timer_ms;
}

bool is_slouch_detected() {
    return is_slouching_state;
}
