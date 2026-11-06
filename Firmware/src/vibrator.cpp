#include "vibrator.h"
#include "config.h"
#include "led.h"
#include <Arduino.h>

static int last_siren_state = -1; 

void vibrator_init() {
    pinMode(PIN_VIBRATOR, OUTPUT);
    digitalWrite(PIN_VIBRATOR, LOW);

    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    led_init(); 
    led_on(); // Bật sáng đèn LED mặc định ngay khi mạch khởi động (trạng thái thẳng lưng)
}

void vibrator_on() {
    // TUYỆT ĐỐI TẮT ĐÈN LED KHI GÙ LƯNG
    led_off();

    uint32_t current_time = millis();
    int current_state = ((current_time / 400) % 2); 
    
    // Chỉ có Motor và Còi là đổi nhịp Wee-Woo
    if (current_state != last_siren_state) {
        last_siren_state = current_state;
        
        if (current_state == 1) {
            tone(PIN_BUZZER, 1000);           
            digitalWrite(PIN_VIBRATOR, HIGH); 
        } else {
            tone(PIN_BUZZER, 700);            
            digitalWrite(PIN_VIBRATOR, LOW);  
        }
    }
}

void vibrator_off() {
    digitalWrite(PIN_VIBRATOR, LOW);
    
    noTone(PIN_BUZZER);            
    pinMode(PIN_BUZZER, OUTPUT);   
    digitalWrite(PIN_BUZZER, LOW); 
    
    // BẬT ĐÈN LED LIÊN TỤC KHI NGỒI THẲNG
    led_on(); 
    last_siren_state = -1;         
}

void vibrator_double_pulse() {
    for (int i = 0; i < 2; i++) {
        digitalWrite(PIN_VIBRATOR, HIGH);
        led_off(); // Tắt LED lúc đang rung phản hồi
        
        delay(150); 
        
        digitalWrite(PIN_VIBRATOR, LOW);
        led_on();  // Bật lại LED khi ngừng rung
        
        if (i == 0) delay(150); 
    }
}