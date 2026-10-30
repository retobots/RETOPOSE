#include "posture_analyzer.h"
#include "config.h"
#include "imu_sensor.h"
#include "vibrator.h"
#include "button.h"
#include "posture_normal.h"
#include "posture_slouch.h"
#include "posture_stretch.h"
#include "posture_drop_pen.h"
#include "posture_walk.h"

static uint32_t last_time_ms = 0;
static float reference_pitch = 0.0f;

void posture_analyzer_init() {
    imu_init();
    vibrator_init();
    button_init(); // Khởi tạo nút nhấn qua Hardware Driver
    
    last_time_ms = millis();
}

void posture_analyzer_run() {
    uint32_t current_time_ms = millis();
    uint32_t dt_ms = current_time_ms - last_time_ms;
    last_time_ms = current_time_ms;

    IMU_Data imu;
    if (imu_read_data(imu)) {
        
        // --- XỬ LÝ NÚT NHẤN ---
        if (is_button_pressed()) {
            reference_pitch = imu.pitch; // Lưu góc chuẩn
        }

        // Tính góc tương đối
        imu.pitch = imu.pitch - reference_pitch;
        
        // Chuỗi ưu tiên xử lý (Logic Tree)
        if (check_walk(imu)) {
            vibrator_off();
        }
        else if (check_drop_pen(imu, dt_ms)) {
            vibrator_off();
        }
        else if (check_slouch(imu, dt_ms)) {
            vibrator_on();
        } 
        else if (check_stretch(imu, dt_ms)) {
            vibrator_off();
        }
        else if (check_normal(imu)) {
            vibrator_off();
        }
    }
}