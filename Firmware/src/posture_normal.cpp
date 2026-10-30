#include "posture_normal.h"
#include "config.h"

bool check_normal(const IMU_Data &imu) {
    // Ngồi chuẩn là khi góc nghiêng trước/sau (pitch) nằm trong biên độ cho phép (+/- 5 độ)
    if (imu.pitch >= -ANGLE_NORMAL_THRESHOLD && imu.pitch <= ANGLE_NORMAL_THRESHOLD) {
        return true; // Đang ngồi đúng tư thế
    }
    return false;
}