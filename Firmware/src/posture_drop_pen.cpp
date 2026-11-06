#include "posture_drop_pen.h"
#include "config.h"
#include <math.h>

static uint32_t drop_pen_timer_ms = 0;
static bool dynamic_dive_detected = false; // Cờ ghi nhận có thao tác gập người nhanh (FR-05)
static bool drop_pen_expired = false;      // Đã quá 3s cúi sâu -> khoá lại, chuyển hẳn sang gù

bool check_drop_pen(float m_delta_pitch, const IMU_Data &imu, uint32_t dt_ms) {
    // 1. Nếu góc cúi lớn (>= 45 độ):
    if (m_delta_pitch >= ANGLE_DROPPEN_THRESHOLD) {
        // Nếu trước đó đã quá 3 giây (đã hết thời hạn nhặt bút) -> khoá, không nhận diện lại
        if (drop_pen_expired) {
            return false;
        }

        drop_pen_timer_ms += dt_ms;

        // BẮT BUỘC PHẢI CÓ CHUYỂN ĐỘNG GẬP NGƯỜI NHANH (FR-05 Gyroscope Dynamic Analysis)
        // - Cúi nhặt bút thật: người gập xuống nhanh (gyro_x lớn > 40 deg/s)
        // - Ngồi gù cúi mặt hoặc thử nghiệm tĩnh: gyro_x xấp xỉ 0 -> TUYỆT ĐỐI KHÔNG coi là nhặt bút!
        if (fabs(imu.gyro_x) >= (GYRO_DYNAMIC_THRESHOLD - 10.0f)) {
            dynamic_dive_detected = true;
        }

        // Chỉ coi là nhặt bút hợp lệ khi:
        // - Có thao tác gập người nhanh (dynamic_dive_detected == true)
        // - VÀ thời gian duy trì chưa quá 3 giây (<= TIME_DROP_PEN_MAX_MS)
        if (dynamic_dive_detected && drop_pen_timer_ms <= TIME_DROP_PEN_MAX_MS) {
            return true; // Tạm thời bỏ qua, không báo động
        }

        // Nếu quá 3 giây hoặc không có chuyển động gập nhanh:
        // Đánh dấu đã hết hạn nhặt bút -> chuyển hẳn sang cảnh báo gù lưng
        if (drop_pen_timer_ms > TIME_DROP_PEN_MAX_MS) {
            drop_pen_expired = true;
        }

        return false;
    } else {
        // Chỉ reset khi người dùng đã thực sự ngẩng người lên khỏi ngưỡng 45 độ
        drop_pen_timer_reset();
        return false;
    }
}

void drop_pen_timer_reset() {
    drop_pen_timer_ms = 0;
    dynamic_dive_detected = false;
    drop_pen_expired = false;
}

uint32_t get_drop_pen_timer_ms() {
    return drop_pen_timer_ms;
}
