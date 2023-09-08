#pragma once

#include "A4988.h"
#include <Servo.h>
#include "./modules/data/data.h"

void check_motors(A4988 &stepper_motor, Servo &servo_motor);
float stepper_motor_logic(float input_degrees, uint16_t &current_postion);
double calculate_azimuth_deg(int32_t object_lat, int32_t object_lon, int32_t tracker_lat, int32_t tracker_lon);
double calculate_elevation_deg(int32_t object_lat, int32_t object_lon, int32_t object_alt, int32_t tracker_lat, int32_t tracker_lon);
double convert_to_degrees(int32_t lat_or_lon );