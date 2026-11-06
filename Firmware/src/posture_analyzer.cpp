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
#include <led.h>

static uint32_t last_time_ms = 0;
static float reference_pitch = 0.0f;
static bool last_button_state = false;

// Biến nội bộ đếm thời gian thiết bị nằm im
static uint32_t idle_timer_ms = 0; 
IMU_Data imu; // Biến toàn cục lưu dữ liệu IMU hiện tại

void posture_analyzer_init() {
    imu_init();
    vibrator_init();
    // button_init();
    led_init();
    last_time_ms = millis();
    // Lấy góc chuẩn ban đầu khi khởi động mạch
    bool imu_data_ready = false;
    unsigned long start_time = millis();
    while (!imu_data_ready) {
        imu_read_data(imu);
        delay(10);
        if (millis() - start_time > 5000) { // Timeout sau 5 giây
            imu_data_ready = true; // Thoát vòng lặp nếu không nhận được dữ liệu
        }
    }
    reference_pitch = imu.pitch; 
    vibrator_double_pulse(); 
    Serial.printf("Reference Pitch set to: %.2f\n", reference_pitch);

}

void posture_analyzer_run() {
    uint32_t current_time_ms = millis();
    uint32_t dt_ms = current_time_ms - last_time_ms;
    last_time_ms = current_time_ms;

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
                
                // === ĐÃ SỬA: Kích hoạt tổng chức năng đánh thức qua GPIO cho ESP32-C3 ===
                esp_deep_sleep_enable_gpio_wakeup(1ULL << PIN_IMU_INT, ESP_GPIO_WAKEUP_GPIO_HIGH);
                // =======================================================================
                
                // Ngủ sâu
                esp_deep_sleep_start(); 
            }
        } else {
            idle_timer_ms = 0; 
        }

        // // --- 2. FR-03: XỬ LÝ NÚT NHẤN LẤY ĐIỂM CHUẨN ---
        // bool current_button_state = is_button_pressed();
        // if (current_button_state == true && last_button_state == false) {
        //     reference_pitch = imu.pitch; 
        //     vibrator_double_pulse();     
        // }
        // last_button_state = current_button_state; 

        // --- 3. TÍNH GÓC TƯƠNG ĐỐI VÀ GỌI CHUỖI LOGIC TƯ THẾ ---
      float delta_pitch = imu.pitch - reference_pitch;
        Serial.printf("Current Pitch: %.2f, Reference Pitch: %.2f, Delta Pitch: %.2f, Accel Total: %.2f, Gyro X: %.2f, Gyro Z: %.2f\n", imu.pitch, reference_pitch, delta_pitch, imu.accel_total, imu.gyro_x, imu.gyro_z);
        
        if (check_walk(imu)) {
            Serial.println("Walking detected");
            vibrator_off();
        }
        else if (check_drop_pen(delta_pitch, imu, dt_ms)) {
            Serial.println("Drop pen detected");
            vibrator_off();
        }
        else if (check_slouch(delta_pitch, imu, dt_ms)) {
            Serial.println("Slouching detected");
            vibrator_on();
            led_on(); // Bật LED khi phát hiện tư thế gù
        } 
        else if (check_stretch(delta_pitch, imu, dt_ms)) {
            Serial.println("Stretching detected");
            vibrator_off();
        }
        else if (check_normal(delta_pitch, imu)) {
            Serial.println("Normal posture detected");
            vibrator_off();
        }
        else {
            Serial.println("Unknown posture detected");

 
        }
    }
}