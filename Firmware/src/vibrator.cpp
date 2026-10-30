#include "vibrator.h"
#include "config.h"

void vibrator_init() {
    pinMode(PIN_VIBRATOR, OUTPUT);
    digitalWrite(PIN_VIBRATOR, LOW);
}

void vibrator_on() {
    digitalWrite(PIN_VIBRATOR, HIGH);
}

void vibrator_off() {
    digitalWrite(PIN_VIBRATOR, LOW);
}

void vibrator_double_pulse() {
    for (int i = 0; i < 2; i++) {
        digitalWrite(PIN_VIBRATOR, HIGH);
        delay(150); // Rung ngắn 150ms
        digitalWrite(PIN_VIBRATOR, LOW);
        if (i == 0) delay(150); // Nghỉ ngắt quãng 150ms giữa 2 nhịp
    }
}