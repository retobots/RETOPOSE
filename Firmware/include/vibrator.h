#ifndef VIBRATOR_H
#define VIBRATOR_H

#include <Arduino.h>

void vibrator_init();
void vibrator_on();
void vibrator_off();
void vibrator_double_pulse();

// Kiểm tra xem hệ thống cảnh báo rung/còi có đang hoạt động hay vừa mới ngắt không
bool is_vibrator_active();
bool is_vibrating_recently(uint32_t ms);

#endif // VIBRATOR_H
