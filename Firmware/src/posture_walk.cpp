#include "posture_walk.h"
#include "config.h"
#include "vibrator.h"
#include <math.h>

static uint32_t walk_hold_timer_ms = 0;
static uint32_t dynamic_motion_ms = 0;

bool check_walk(const IMU_Data &imu, float delta_pitch, uint32_t dt_ms) {
    // 1. Nếu còi/motor rung đang kích hoạt hoặc vừa ngắt (trong 400ms):
    // Cảm biến IMU đang chịu rung chấn cơ học từ motor, TUYỆT ĐỐI KHÔNG nhận diện nhầm là đi bộ!
    if (is_vibrating_recently(400)) {
        walk_hold_timer_ms = 0;
        dynamic_motion_ms = 0;
        return false;
    }

    // 2. Nếu người dùng đang ngồi gù lưng rõ rệt (delta_pitch > ngưỡng gù):
    // Người dùng đang ngồi cúi gù tại chỗ, không thể là đang đi bộ bình thường!
    if (delta_pitch > ANGLE_SLOUCH_THRESHOLD) {
        walk_hold_timer_ms = 0;
        dynamic_motion_ms = 0;
        return false;
    }

    // 3. Nhận diện đi bộ hoặc đứng dậy qua biến thiên gia tốc và xoay mình (FR-06)
    bool motion_detected = (fabs(imu.accel_total - 1.0f) > 0.25f) ||
                           (fabs(imu.gyro_y) > 35.0f) ||
                           (fabs(imu.gyro_z) > 35.0f);

    if (motion_detected) {
        dynamic_motion_ms += dt_ms;
        // Cần duy trì ít nhất 80ms chuyển động liên tục để loại bỏ rung lắc ngẫu nhiên
        if (dynamic_motion_ms >= 80) {
            walk_hold_timer_ms = 1000; // Giữ trạng thái Standby trong 1 giây để nối các bước đi
            return true;
        }
    } else {
        dynamic_motion_ms = 0;
    }

    if (walk_hold_timer_ms > 0) {
        if (walk_hold_timer_ms > dt_ms) {
            walk_hold_timer_ms -= dt_ms;
        } else {
            walk_hold_timer_ms = 0;
        }
        return true;
    }

    return false;
}

void walk_timer_reset() {
    walk_hold_timer_ms = 0;
    dynamic_motion_ms = 0;
}
