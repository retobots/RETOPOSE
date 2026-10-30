#include "vibrator.h"
#include "config.h"

void vibrator_init() {
    pinMode(PIN_VIBRATOR, OUTPUT);
    digitalWrite(PIN_VIBRATOR, LOW); // Đảm bảo lúc mới bật máy thì không rung
}

void vibrator_on() {
    digitalWrite(PIN_VIBRATOR, HIGH);
}

void vibrator_off() {
    digitalWrite(PIN_VIBRATOR, LOW);
}