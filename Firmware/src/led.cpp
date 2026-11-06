#include "led.h"
#include "config.h"
#include <Arduino.h>

void led_init() {
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW); // Tắt LED khi khởi động
}

void led_on() {
    digitalWrite(PIN_LED, HIGH);
}

void led_off() {
    digitalWrite(PIN_LED, LOW);
}

void led_toggle() {
    digitalWrite(PIN_LED, !digitalRead(PIN_LED));
}