#include "posture_walk.h"
#include "config.h"

// Ngưỡng gia tốc khi đi bộ (tạm gán 1.2g)
constexpr float WALK_ACCEL_THRESHOLD = 1.2f;

bool check_walk(const IMU_Data &imu) {
    // Nếu gia tốc tổng hợp vượt ngưỡng -> Đang đi bộ
    if (imu.accel_total >= WALK_ACCEL_THRESHOLD) {
        return true;
    }
    return false;
}