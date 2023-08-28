#pragma once

#include <Arduino.h>

typedef struct{
    //data input
    int32_t alt;
    int32_t relative_alt;
    uint16_t hdg; 
    uint16_t cpu_load; 
    uint16_t drop_rate_comm;
    int32_t lat;
    int32_t lon;
    uint8_t satellites_visible;
    uint8_t fix_type;
    uint16_t cog;
    uint16_t vel;
    uint8_t rssi;
    //data buffers
    int alt_buffer;
    int relative_alt_buffer;
    int hdg_buffer;
    int cpu_load_buffer; 
    int drop_rate_comm_buffer;
    int lat_buffer;
    int lon_buffer;
    int satellites_visible_buffer;
    int fix_type_buffer;
    int cog_buffer;
    int vel_buffer;
    int rssi_buffer;
}telemetry_data;