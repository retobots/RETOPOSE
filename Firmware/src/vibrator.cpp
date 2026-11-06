#include "vibrator.h"
#include "config.h"
#include "led.h"
#include <Arduino.h>

static int last_siren_phase = -1;
static bool is_alarm_mode = false;
static uint32_t last_vibrator_stop_time = 0;

void vibrator_init() {
    pinMode(PIN_VIBRATOR, OUTPUT);
    digitalWrite(PIN_VIBRATOR, LOW);

    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    led_init();
    led_on(); // Bật sáng đèn LED liên tục mặc định khi khởi động (trạng thái ngồi thẳng)
    is_alarm_mode = false;
    last_vibrator_stop_time = 0;
}

void vibrator_on() {
    is_alarm_mode = true;
    uint32_t current_time = millis();

    // 1. Còi phát âm thanh xe cứu thương (Ambulance siren Wee-Woo: 950Hz và 700Hz)
    // 2. Động cơ rung gián đoạn: Rung trong pha 1 (350ms), ngắt trong pha 0 (350ms)
    int current_phase = ((current_time / 350) % 2);

    if (current_phase != last_siren_phase) {
        last_siren_phase = current_phase;

        if (current_phase == 1) {
            tone(PIN_BUZZER, 950);            // Âm cao
            digitalWrite(PIN_VIBRATOR, HIGH); // Kích hoạt motor rung
        } else {
            tone(PIN_BUZZER, 700);            // Âm trầm
            digitalWrite(PIN_VIBRATOR, LOW);  // Ngắt motor rung (tạo rung gián đoạn)
        }
    }

    // 3. Đèn LED nhấp nháy liên tục (chu kỳ 150ms tạo hiệu ứng chớp cảnh báo)
    if (((current_time / 150) % 2) == 0) {
        led_on();
    } else {
        led_off();
    }
}

void vibrator_off() {
    if (is_alarm_mode) {
        last_vibrator_stop_time = millis();
    }
    is_alarm_mode = false;

    digitalWrite(PIN_VIBRATOR, LOW);

    noTone(PIN_BUZZER);
    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    // Trở về trạng thái ngồi thẳng: LED sáng liên tục
    led_on();
    last_siren_phase = -1;
}

void vibrator_double_pulse() {
    for (int i = 0; i < 2; i++) {
        digitalWrite(PIN_VIBRATOR, HIGH);
        tone(PIN_BUZZER, 1200);
        led_off();

        delay(120);

        digitalWrite(PIN_VIBRATOR, LOW);
        noTone(PIN_BUZZER);
        led_on();

        if (i == 0) delay(120);
    }
    last_vibrator_stop_time = millis();
}

bool is_vibrator_active() {
    return is_alarm_mode;
}

bool is_vibrating_recently(uint32_t ms) {
    if (is_alarm_mode) return true;
    return ((millis() - last_vibrator_stop_time) < ms);
}
