#include "posture_normal.h"
#include "config.h"

bool check_normal(float m_delta_pitch, const IMU_Data &imu) {
    // Ngồi chuẩn là khi góc nghiêng trước/sau (pitch) nằm trong biên độ cho phép (+/- 5 độ)
    if (m_delta_pitch >= -ANGLE_NORMAL_THRESHOLD && m_delta_pitch <= ANGLE_NORMAL_THRESHOLD) {
        return true; // Đang ngồi đúng tư thế
    }
    return false;
}