#ifndef VIBRATOR_H
#define VIBRATOR_H

#include <Arduino.h>

void vibrator_init();
void vibrator_on();
void vibrator_off();

void vibrator_pulse(uint32_t duration_ms);

#endif