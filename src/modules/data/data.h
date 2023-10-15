#pragma once

#include <Arduino.h>
#include "./modules/kinematics/kinematics.h"

typedef struct{
    // data input
    int32_t alt;
    int32_t relative_alt;
    uint16_t drop_rate_comm;
    int32_t lat;
    int32_t lon;
    uint8_t satellites_visible;
    uint8_t fix_type;
    uint8_t rssi;
    // data buffers
    uint8_t data_counter;
    int alt_buffer;
    int relative_alt_buffer;
    int drop_rate_comm_buffer;
    int lat_buffer;
    int lon_buffer;
    int satellites_visible_buffer;
    int fix_type_buffer;
    int rssi_buffer;
    // average data
    int32_t average_alt;
    int32_t average_relative_alt;
    uint16_t average_drop_rate_comm;
    int32_t average_lat;
    int32_t average_lon;
    uint8_t average_satellites_visible;
    uint8_t average_fix_type;
    uint8_t average_rssi;
    // postion/movement data
    uint16_t stepper_motor_postion;
    int32_t tracker_lat;
    int32_t tracker_lon;

}telemetry_data;

void process_data(telemetry_data *data, uint8_t calibration_flag, A4988 &stepper_motor, Servo &servo_motor);
void set_tracker_postion(telemetry_data *data);
