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
#include <esp_sleep.h> 
#include <driver/gpio.h> 

static uint32_t last_time_ms = 0;
static float reference_pitch = 0.0f;
static bool last_button_state = false;

// Biến nội bộ đếm thời gian thiết bị nằm im
static uint32_t idle_timer_ms = 0; 

void posture_analyzer_init() {
    imu_init();
    vibrator_init();
    button_init();
    last_time_ms = millis();
}

void posture_analyzer_run() {
    uint32_t current_time_ms = millis();
    uint32_t dt_ms = current_time_ms - last_time_ms;
    last_time_ms = current_time_ms;

    IMU_Data imu;
    if (imu_read_data(imu)) {
        
        // --- 1. FR-09 & FR-10: KIỂM TRA NẰM IM ĐỂ NGỦ SÂU & ĐÁNH THỨC KÉP ---
        if (abs(imu.accel_total - 1.0f) < ACCEL_STATIC_THRESHOLD && 
            abs(imu.gyro_z) < GYRO_STATIC_THRESHOLD) {
            
            idle_timer_ms += dt_ms;
            
            if (idle_timer_ms >= TIME_IDLE_DEEP_SLEEP_MS) {
                vibrator_on(); delay(500); vibrator_off(); 
                
                // Lên đạn cho cảm biến MPU canh gác
                imu_enable_wake_on_motion();

                // Cấu hình đánh thức bằng Nút nhấn (Trạng thái kéo xuống LOW)
                gpio_wakeup_enable((gpio_num_t)PIN_BUTTON_RESET, GPIO_INTR_LOW_LEVEL);
                
                // Cấu hình đánh thức bằng Cảm biến (Trạng thái đẩy lên HIGH)
                pinMode(PIN_IMU_INT, INPUT_PULLDOWN); 
                gpio_wakeup_enable((gpio_num_t)PIN_IMU_INT, GPIO_INTR_HIGH_LEVEL);
                
                // Kích hoạt tính năng đánh thức kép
                esp_deep_sleep_enable_gpio_wakeup(); 
                
                // Ngủ sâu
                esp_deep_sleep_start(); 
            }
        } else {
            idle_timer_ms = 0; 
        }

        // --- 2. FR-03: XỬ LÝ NÚT NHẤN LẤY ĐIỂM CHUẨN ---
        bool current_button_state = is_button_pressed();
        if (current_button_state == true && last_button_state == false) {
            reference_pitch = imu.pitch; 
            vibrator_double_pulse();     
        }
        last_button_state = current_button_state; 

        // --- 3. TÍNH GÓC TƯƠNG ĐỐI VÀ GỌI CHUỖI LOGIC TƯ THẾ ---
        imu.pitch = imu.pitch - reference_pitch;
        
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