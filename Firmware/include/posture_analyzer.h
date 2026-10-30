#ifndef POSTURE_ANALYZER_H
#define POSTURE_ANALYZER_H

#include <Arduino.h>

void posture_analyzer_init(); //goi imu va vibrator

void posture_analyzer_run(); //vong lap phan tich: doc imu check bang 5 tinh huong, duoc goi lien tuc ben trong ham loop() cua main.cpp

#endif