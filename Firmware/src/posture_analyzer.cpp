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
#include "led.h"
#include <string.h>

static uint32_t last_time_ms = 0;
static float reference_pitch = 0.0f;
static bool last_button_state = false;

// Biến nội bộ đếm thời gian thiết bị nằm im
static uint32_t idle_timer_ms = 0; 
IMU_Data imu; // Dữ liệu IMU hiện tại

// Định nghĩa các trạng thái nhận diện tư thế
enum PostureState {
    STATE_UNKNOWN,
    STATE_NORMAL,
    STATE_BUFFER_ZONE,
    STATE_WALKING,
    STATE_DROP_PEN,
    STATE_STRETCHING,
    STATE_ALARM_STRETCH_TIMEOUT,
    STATE_SLOUCH_COUNTING,
    STATE_ALARM_SLOUCH
};

static PostureState current_state = STATE_UNKNOWN;
static PostureState last_state = STATE_UNKNOWN;

static const char* state_to_string(PostureState state) {
    switch (state) {
        case STATE_NORMAL:                 return "NORMAL";
        case STATE_BUFFER_ZONE:            return "BUFFER_ZONE (5-10 deg)";
        case STATE_WALKING:                return "WALKING";
        case STATE_DROP_PEN:               return "DROP_PEN (<= 3s)";
        case STATE_STRETCHING:             return "STRETCHING (<= 60s)";
        case STATE_ALARM_STRETCH_TIMEOUT:  return "ALARM_STRETCH (> 60s)";
        case STATE_SLOUCH_COUNTING:        return "SLOUCH_COUNTING (0-5s)";
        case STATE_ALARM_SLOUCH:           return "ALARM_SLOUCH (> 5s)";
        default:                           return "UNKNOWN";
    }
}

void posture_analyzer_init() {
    Serial.println("\n=========================================");
    Serial.println("   RETOPOSE - SMART POSTURE CORRECTOR   ");
    Serial.println("=========================================");
    Serial.println("[SYSTEM] Dang khoi tao phan cung...");

    vibrator_init();
    button_init();
    led_init();

    if (!imu_init()) {
        Serial.println("[ERROR] Khong the ket noi MPU6050 qua I2C! Kiem tra day noi.");
        for (int i = 0; i < 6; i++) {
            led_on(); delay(80); led_off(); delay(80);
        }
    } else {
        Serial.println("[OK] MPU6050 da khoi tao va can bang Gyro thanh cong.");
    }

    last_time_ms = millis();

    // Lấy góc chuẩn ban đầu bằng trung bình mẫu trong 1.5 giây
    Serial.println("[CALIB] Dang do goc ngoi thang ban dau trong 1.5s...");
    float pitch_sum = 0.0f;
    int valid_samples = 0;
    unsigned long start_time = millis();
    while (millis() - start_time < 1500) {
        if (imu_read_data(imu)) {
            pitch_sum += imu.pitch;
            valid_samples++;
        }
        delay(10);
    }

    if (valid_samples > 0) {
        reference_pitch = pitch_sum / valid_samples;
    } else {
        reference_pitch = imu.pitch;
    }

    // Phát 2 nhịp rung/beep xác nhận hiệu chuẩn thành công (FR-03)
    vibrator_double_pulse(); 
    Serial.printf("[CALIB] Hoan tat! Goc tham chieu (Reference Pitch): %.2f deg\n", reference_pitch);
    Serial.println("[SYSTEM] Bat dau vong lap giam sat tu the...\n");
}

void posture_analyzer_run() {
    uint32_t current_time_ms = millis();
    uint32_t dt_ms = current_time_ms - last_time_ms;
    last_time_ms = current_time_ms;

    if (!imu_read_data(imu)) {
        return; // Đọc dữ liệu lỗi, bỏ qua chu kỳ này
    }

    // --- 1. XỬ LÝ NÚT NHẤN HIỆU CHUẨN TƯ THẾ (FR-01, FR-02, FR-03) ---
    bool current_button_state = is_button_pressed();
    if (current_button_state && !last_button_state) {
        reference_pitch = imu.pitch;
        vibrator_double_pulse();     
        slouch_timer_reset();
        drop_pen_timer_reset();
        stretch_timer_reset();
        walk_timer_reset();
        vibrator_off();
        Serial.printf("\n>>> [NUT NHAN] DA HIỆU CHUẨN LẠI! Reference Pitch = %.2f deg\n\n", reference_pitch);
    }
    last_button_state = current_button_state; 

    // --- 2. KIỂM TRA NẰM IM ĐỂ NGỦ SÂU (DEEP SLEEP) (FR-09 & FR-10) ---
    float max_gyro = max(fabs(imu.gyro_x), max(fabs(imu.gyro_y), fabs(imu.gyro_z)));
    if (fabs(imu.accel_total - 1.0f) < ACCEL_STATIC_THRESHOLD && max_gyro < GYRO_STATIC_THRESHOLD) {
        idle_timer_ms += dt_ms;
        if (idle_timer_ms >= TIME_IDLE_DEEP_SLEEP_MS) {
            Serial.println("\n[SLEEP] Thiet bi nam im qua 3 phut. Chuyen vao Deep Sleep...");
            vibrator_off();
            led_off();

            digitalWrite(PIN_VIBRATOR, HIGH);
            delay(200);
            digitalWrite(PIN_VIBRATOR, LOW);

            imu_enable_wake_on_motion();
            pinMode(PIN_IMU_INT, INPUT_PULLDOWN);
            esp_deep_sleep_enable_gpio_wakeup(1ULL << PIN_IMU_INT, ESP_GPIO_WAKEUP_GPIO_HIGH);

            esp_deep_sleep_start();
        }
    } else {
        idle_timer_ms = 0; 
    }

    // --- 3. TÍNH GÓC TƯƠNG ĐỐI VÀ XÁC ĐỊNH TRẠNG THÁI ---
    float delta_pitch = imu.pitch - reference_pitch;

    // Ưu tiên 1 - Chuyển động toàn thân (Đi lại / Đứng dậy)
    if (check_walk(imu, delta_pitch, dt_ms)) {
        current_state = STATE_WALKING;
        vibrator_off();
        slouch_timer_reset();
        stretch_timer_reset();
        if (delta_pitch < ANGLE_DROPPEN_THRESHOLD) drop_pen_timer_reset();
    }
    // Ưu tiên 2 - Hành vi tạm thời (Cúi nhặt bút <= 3 giây)
    else if (check_drop_pen(delta_pitch, imu, dt_ms)) {
        current_state = STATE_DROP_PEN;
        vibrator_off();
        slouch_timer_reset();
        stretch_timer_reset();
    }
    // Ưu tiên 3 - Ngả lưng quá thời gian cho phép (> 60s ngủ gật)
    else if (is_stretch_timeout()) {
        current_state = STATE_ALARM_STRETCH_TIMEOUT;
        vibrator_on();
    }
    // Ưu tiên 4 - Ngả lưng vươn vai thư giãn hợp lệ (<= 60 giây)
    else if (check_stretch(delta_pitch, imu, dt_ms)) {
        current_state = STATE_STRETCHING;
        vibrator_off();
        slouch_timer_reset();
        if (delta_pitch < ANGLE_DROPPEN_THRESHOLD) drop_pen_timer_reset();
    }
    // Ưu tiên 5 - Gù lưng kéo dài (> 5 giây) -> Kích hoạt BÁO ĐỘNG
    else if (check_slouch(delta_pitch, imu, dt_ms)) {
        current_state = STATE_ALARM_SLOUCH;
        vibrator_on(); // Rung gián đoạn + Còi cứu thương + LED nhấp nháy
        stretch_timer_reset();
        if (delta_pitch < ANGLE_DROPPEN_THRESHOLD) drop_pen_timer_reset();
    }
    // Đang phát hiện góc gù nhưng chưa đủ 5 giây (đang đếm trễ)
    else if (is_slouch_detected()) {
        current_state = STATE_SLOUCH_COUNTING;
        vibrator_off();
    }
    // Ưu tiên 6 - Tư thế chuẩn (±5 độ)
    else if (check_normal(delta_pitch, imu)) {
        current_state = STATE_NORMAL;
        vibrator_off();
        slouch_timer_reset();
        drop_pen_timer_reset();
        stretch_timer_reset();
    }
    // Vùng đệm chuyển tiếp (giữa 5 và 10 độ)
    else {
        current_state = STATE_BUFFER_ZONE;
        vibrator_off();
    }

    // --- 4. LOG SERIAL THÔNG MINH CHO DEBUG DỄ DÀNG ---

    // A. In NGAY LẬP TỨC khi phát hiện CHUYỂN ĐỔI TRẠNG THÁI (Event Transition)
    if (current_state != last_state) {
        Serial.printf(">>> [STATE CHANGED] %s ===> %s | Delta: %+.1f deg (Pitch: %.1f, Ref: %.1f)\n",
                      state_to_string(last_state), state_to_string(current_state),
                      delta_pitch, imu.pitch, reference_pitch);
        last_state = current_state;
    }

    // B. In định kỳ mỗi 400ms để theo dõi liên tục tiến độ các Timer và thông số cảm biến
    static uint32_t last_periodic_log = 0;
    if (current_time_ms - last_periodic_log >= 400) {
        last_periodic_log = current_time_ms;

        switch (current_state) {
            case STATE_NORMAL:
                Serial.printf("[STATUS] %-18s | Tu the chuan [OK]      | Delta: %+.1f deg | Pitch: %.1f deg | Led: ON\n",
                              "[NORMAL]", delta_pitch, imu.pitch);
                break;

            case STATE_SLOUCH_COUNTING:
                Serial.printf("[STATUS] %-18s | Dang dem gu: %4ums/%ums | Delta: %+.1f deg | Pitch: %.1f deg | GyrX: %+.1f\n",
                              "[SLOUCH COUNT]", get_slouch_timer_ms(), TIME_SLOUCH_MS, delta_pitch, imu.pitch, imu.gyro_x);
                break;

            case STATE_ALARM_SLOUCH:
                Serial.printf("[STATUS] %-18s | !!! BAO DONG GU LUNG !!! | Delta: %+.1f deg | Vibe: PULSE | Buzzer: SIREN | Led: BLINK\n",
                              "[ALARM SLOUCH]", delta_pitch);
                break;

            case STATE_DROP_PEN:
                Serial.printf("[STATUS] %-18s | Cui nhat do (tam bo qua) | Timer: %4ums/%ums | Delta: %+.1f deg | Acc: %.2fg\n",
                              "[DROP PEN]", get_drop_pen_timer_ms(), TIME_DROP_PEN_MAX_MS, delta_pitch, imu.accel_total);
                break;

            case STATE_WALKING:
                Serial.printf("[STATUS] %-18s | Van dong / Di lai       | Acc: %.2fg | Standby khong bao dong\n",
                              "[WALK]", imu.accel_total);
                break;

            case STATE_STRETCHING:
                Serial.printf("[STATUS] %-18s | Vuon vai thu gian       | Timer: %5ums/%ums | Delta: %+.1f deg\n",
                              "[STRETCH]", get_stretch_timer_ms(), TIME_STRETCH_MAX_MS, delta_pitch);
                break;

            case STATE_ALARM_STRETCH_TIMEOUT:
                Serial.printf("[STATUS] %-18s | !!! BAO DONG NGU GAT !!! | Vuon vai qua 60s | Delta: %+.1f deg\n",
                              "[ALARM SLEEP]", delta_pitch);
                break;

            case STATE_BUFFER_ZONE:
                Serial.printf("[STATUS] %-18s | Vung dem chuyen tiep    | Delta: %+.1f deg | Pitch: %.1f deg\n",
                              "[BUFFER ZONE]", delta_pitch, imu.pitch);
                break;

            default:
                break;
        }
    }
}
