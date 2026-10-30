#include "posture_walk.h"
#include "config.h"

bool check_walk(const IMU_Data &imu) {
    // Nếu gia tốc tổng hợp vượt ngưỡng -> Đang đi bộ / di chuyển toàn thân (FR-06)
    if (imu.accel_total >= WALK_ACCEL_THRESHOLD) {
        return true;
    }
    return false;
}