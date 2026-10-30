#ifndef VIBRATOR_H
#define VIBRATOR_H

#include <Arduino.h>

void vibrator_init();
void vibrator_on();
void vibrator_off();
void vibrator_double_pulse(); // Khai báo hàm rung xác nhận

#endif // VIBRATOR_H