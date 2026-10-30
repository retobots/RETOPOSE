#include <Arduino.h>
#include "posture_analyzer.h"

void setup() {
    // Khởi tạo toàn bộ hệ thống (I2C, Cục rung, Nút nhấn)
    posture_analyzer_init();
}

void loop() {
    // Chạy vòng lặp phân tích tư thế liên tục
    posture_analyzer_run();
    
    // Nghỉ 10ms mỗi chu kỳ để chip ESP32-C3 không bị quá tải (Watchdog Reset)
    delay(10); 
}