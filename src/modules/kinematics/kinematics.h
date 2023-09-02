#pragma once

#include "A4988.h"
#include <Servo.h>

void check_motors(A4988 &stepper_motor, Servo &servo_motor);
float stepper_motor_logic(float input_degrees, uint16_t &current_postion);