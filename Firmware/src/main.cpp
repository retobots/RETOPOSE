#include <Arduino.h>
#include "posture_analyzer.h"

void setup() {
    Serial.begin(115200); 
    delay(2000); // Đợi 2 giây để cổng USB trên máy tính nhận diện mạch
    Serial.println("====== MACH ESP32-C3 DA KHOI DONG ======");
    // Khởi tạo toàn bộ hệ thống (I2C, Cục rung, Nút nhấn)
    posture_analyzer_init();
}

void loop() {
    // Chạy vòng lặp phân tích tư thế liên tục
    posture_analyzer_run();
    
    // Nghỉ 10ms mỗi chu kỳ để chip ESP32-C3 không bị quá tải (Watchdog Reset)
    delay(10); 
}