#include "button.h"
#include "config.h"

void button_init() {
    // Trạng thái bình thường là HIGH, bấm xuống là LOW
    pinMode(PIN_BUTTON_RESET, INPUT_PULLUP); 
}

bool is_button_pressed() {
    // Trả về true nếu nút đang bị nhấn (kéo xuống LOW)
    return digitalRead(PIN_BUTTON_RESET) == LOW;
}