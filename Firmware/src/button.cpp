#include "button.h"
#include "config.h"

void button_init() {
    // Trạng thái bình thường là HIGH, bấm xuống là LOW
    pinMode(PIN_BUTTON_RESET, INPUT_PULLUP); 
}

bool is_button_pressed() {
    // Nếu phát hiện nút bị nhấn (kéo xuống LOW)
    if (digitalRead(PIN_BUTTON_RESET) == LOW) {
        
        // CHỐNG DỘI PHÍM: Chờ 50 mili-giây để lò xo nút nhấn ổn định
        delay(50);
        
        // Kiểm tra lại lần nữa, nếu vẫn giữ trạng thái LOW thì chắc chắn là đang bấm
        if (digitalRead(PIN_BUTTON_RESET) == LOW) {
            return true;
        }
    }
    
    return false;
}