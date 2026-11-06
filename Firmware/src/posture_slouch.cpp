#include "posture_slouch.h"
#include "config.h"
#include <math.h>

static uint32_t slouch_timer_ms = 0;
static bool is_slouching_state = false; 

bool check_slouch(float m_delta_pitch, const IMU_Data& data, uint32_t dt_ms) {
    if (m_delta_pitch > ANGLE_SLOUCH_THRESHOLD) {
        is_slouching_state = true;
    } else if (m_delta_pitch < (ANGLE_SLOUCH_THRESHOLD - 3.0f)) { 
        is_slouching_state = false;
    }

    if (is_slouching_state) {
        // Chỉ cộng dồn thời gian khi người dùng tĩnh lặng (không vặn mình mạnh)
        // Nếu vặn mình, bộ đếm chỉ tạm dừng chứ KHÔNG ngắt ngang báo động
        if (abs(data.gyro_x) <= GYRO_DYNAMIC_THRESHOLD) {
            slouch_timer_ms += dt_ms;
        }
        
        // Kích hoạt báo động khi đủ thời gian
        if (slouch_timer_ms >= TIME_SLOUCH_MS) {
            return true; 
        }
    } else {
        // Chỉ reset thời gian khi đã nhấc người thẳng lên hoàn toàn (< 12 độ)
        slouch_timer_reset();
    }
    
    return false;
}

void slouch_timer_reset() {
    slouch_timer_ms = 0;
    is_slouching_state = false;
}