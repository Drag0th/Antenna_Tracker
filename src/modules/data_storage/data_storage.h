#pragma once
#include <Arduino.h>
#include "../config.h"

typedef struct {
    uint8_t loop_counter;
    int tracker_lat, tracker_lon;
    //MAVLINK_GLOBAL_POSITION
    int32_t relative_alt_array[TELEMTRY_DATA_SAMPLES-1];
    uint16_t hdg_array[TELEMTRY_DATA_SAMPLES-1];
    int average_relative_alt, average_hdg, average_relative_alt_backup;
    //__MAVLINK_SYS_STATUS_T
    int8_t battery_remaining_array[TELEMTRY_DATA_SAMPLES-1];
    uint16_t current_battery_array[TELEMTRY_DATA_SAMPLES-1], voltage_battery_array[TELEMTRY_DATA_SAMPLES-1], cpu_load_array[TELEMTRY_DATA_SAMPLES-1], drop_rate_comm_array[TELEMTRY_DATA_SAMPLES-1];
    int average_battery_remaining, average_current_battery, average_voltage_battery, average_cpu_load, average_drop_rate_comm;
    //MAVLINK_GPS_RAW_INT_T
    int32_t lat_array[TELEMTRY_DATA_SAMPLES-1], lon_array[TELEMTRY_DATA_SAMPLES-1];
    uint8_t satellites_visible_array[TELEMTRY_DATA_SAMPLES-1], fix_type_array[TELEMTRY_DATA_SAMPLES-1];
    uint16_t cog_array[TELEMTRY_DATA_SAMPLES-1], vel_array[TELEMTRY_DATA_SAMPLES-1];
    int average_lat, average_lon, average_satellites_visible, average_fix_type, average_cog, average_vel, average_lat_backup, average_lon_backup;
    //MAVLINK_RC_CHANNELS_RAW_T
    uint8_t rssi_array[TELEMTRY_DATA_SAMPLES-1];
    int average_rssi;
}telemetry_data;

void process_telemetry_data(telemetry_data *data);

double calculate_azimuth_deg(int32_t object_lat, int32_t object_lon, int32_t tracker_lat, int32_t tracker_lon);

double calculate_elevation_deg(int32_t object_lat, int32_t object_lon, int32_t object_alt, int32_t tracker_lat, int32_t tracker_lon);

double convert_to_degrees(int32_t input);

