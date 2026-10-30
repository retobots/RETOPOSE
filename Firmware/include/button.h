#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// Cấu hình chân GPIO cho nút nhấn
void button_init();

// Kiểm tra xem nút có đang được bấm hay không
bool is_button_pressed();

#endif // BUTTON_H