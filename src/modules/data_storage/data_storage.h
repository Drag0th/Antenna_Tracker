#pragma once
#include <Arduino.h>

#define ARRAY_SIZE 10

typedef struct {
    uint8_t loop_counter;
    //MAVLINK_GLOBAL_POSITION
    int32_t relative_alt_array[ARRAY_SIZE-1];
    uint16_t hdg_array[ARRAY_SIZE-1];
    int average_relative_alt, average_hdg;
    //__MAVLINK_SYS_STATUS_T
    int8_t battery_remaining_array[ARRAY_SIZE-1];
    uint16_t current_battery_array[ARRAY_SIZE-1], voltage_battery_array[ARRAY_SIZE-1], cpu_load_array[ARRAY_SIZE-1], drop_rate_comm_array[ARRAY_SIZE-1];
    int average_battery_remaining, average_current_battery, average_voltage_battery, average_cpu_load, average_drop_rate_comm;
    //MAVLINK_GPS_RAW_INT_T
    int32_t lat_array[ARRAY_SIZE-1], lon_array[ARRAY_SIZE-1];
    uint8_t satellites_visible_array[ARRAY_SIZE-1], fix_type_array[ARRAY_SIZE-1];
    uint16_t cog_array[ARRAY_SIZE-1], vel_array[ARRAY_SIZE-1];
    int average_lat, average_lon, average_satellites_visible, average_fix_type, average_cog, average_vel;
    //MAVLINK_RC_CHANNELS_RAW_T
    uint8_t rssi_array[ARRAY_SIZE-1];
    int average_rssi;
}telemetry_data;

void process_telemetry_data(telemetry_data *data);

