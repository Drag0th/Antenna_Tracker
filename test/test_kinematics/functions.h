#pragma once

#include <cstdint>
#include <cmath>
#include <config.h>

double convert_to_degrees(int32_t lat_or_lon);
double stepper_motor_logic(double input_degrees, double current_postion);
double calculate_azimuth_deg(int32_t object_lat, int32_t object_lon, int32_t tracker_lat, int32_t tracker_lon);
double calculate_elevation_deg(int32_t object_lat, int32_t object_lon, int32_t object_alt, int32_t tracker_lat, int32_t tracker_lon);