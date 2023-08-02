#pragma once
#include <Arduino.h>

#define ARRAY_SIZE 10

class dataStorage{
    private:       
        int array_counter = 0;
        //mavlink_global_position
        int32_t relative_alt[ARRAY_SIZE-1];
        uint16_t hdg[ARRAY_SIZE-1];
        //__mavlink_sys_status_t
        int8_t battery_remaining[ARRAY_SIZE-1];
        uint16_t current_battery[ARRAY_SIZE-1], voltage_battery[ARRAY_SIZE-1], cpu_load[ARRAY_SIZE-1], drop_rate_comm[ARRAY_SIZE-1];
        //mavlink_gps_raw_int_t
        int32_t lat[ARRAY_SIZE-1], lon[ARRAY_SIZE-1];
        uint8_t satellites_visible[ARRAY_SIZE-1], fix_type[ARRAY_SIZE-1];
        uint16_t cog[ARRAY_SIZE-1], vel[ARRAY_SIZE-1];
        //mavlink_rc_channels_raw_t
        uint8_t rssi[ARRAY_SIZE-1];
        void processData();
    public:
        //mavlink_global_position
        int average_relative_alt, average_hdg;
        //__mavlink_sys_status_t
        int average_battery_remaining, average_current_battery, average_voltage_battery, average_cpu_load, average_drop_rate_comm;
        //mavlink_gps_raw_int_t
        int average_lat, average_lon, average_satellites_visible, average_fix_type, average_cog, average_vel;
        //mavlink_rc_channels_raw_t
        int average_rssi;

        void getData(int32_t relative_alt_data, uint16_t hdg_data, int8_t battery_remaining_data, uint16_t current_battery_data, uint16_t voltage_battery_data, uint16_t cpu_load_data, uint16_t drop_rate_comm_data, int32_t lat_data, int32_t lon_data, uint8_t satellites_visible_data, uint8_t fix_type_data, uint16_t cog_data, uint16_t vel_data, uint8_t rssi_data);
        
};