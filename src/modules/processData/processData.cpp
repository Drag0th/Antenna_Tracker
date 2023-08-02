#include "processData.h"

void dataStorage::getData(int32_t relative_alt_data, uint16_t hdg_data, int8_t battery_remaining_data, uint16_t current_battery_data, uint16_t voltage_battery_data, uint16_t cpu_load_data, uint16_t drop_rate_comm_data, int32_t lat_data, int32_t lon_data, uint8_t satellites_visible_data, uint8_t fix_type_data, uint16_t cog_data, uint16_t vel_data, uint8_t rssi_data){
    
    relative_alt[array_counter] = relative_alt_data;
    hdg[array_counter] = hdg_data;
    battery_remaining[array_counter] = battery_remaining_data;
    current_battery[array_counter] = current_battery_data;
    voltage_battery[array_counter] = voltage_battery_data;
    cpu_load[array_counter] = cpu_load_data;
    drop_rate_comm[array_counter] = drop_rate_comm_data;
    lat[array_counter] = lat_data;
    lon[array_counter] = lon_data;
    satellites_visible[array_counter] = satellites_visible_data;
    cog[array_counter] = cog_data;
    vel[array_counter] = vel_data;
    rssi[array_counter] = rssi_data;

    if(array_counter == 9){
        processData();
        array_counter = 0;
    }
    else{
        array_counter++;
    }
};

void dataStorage::processData(){

    for(int i = 0 ; i < 10 ; i++){
        average_relative_alt = average_relative_alt + relative_alt[i];
        average_hdg = average_hdg + hdg[i];
        average_battery_remaining = average_battery_remaining + battery_remaining[i];
        average_current_battery = average_current_battery + current_battery[i];
        average_voltage_battery = average_voltage_battery + voltage_battery[i];
        average_cpu_load = average_cpu_load + cpu_load[i];
        average_drop_rate_comm = average_drop_rate_comm + drop_rate_comm[i];
        average_lat = average_lat + lat[i];
        average_lon = average_lon + lon[i];
        average_satellites_visible = average_satellites_visible + satellites_visible[i];
        average_cog = average_cog + cog[i];
        average_vel = average_vel + vel[i];
        average_rssi = average_rssi + rssi[i];
    }

    average_relative_alt /= ARRAY_SIZE;
    average_hdg /= ARRAY_SIZE;
    average_battery_remaining /= ARRAY_SIZE;
    average_current_battery /= ARRAY_SIZE;
    average_voltage_battery /= ARRAY_SIZE;
    average_cpu_load /= ARRAY_SIZE;
    average_drop_rate_comm /= ARRAY_SIZE;
    average_lat /= ARRAY_SIZE;
    average_lon /= ARRAY_SIZE;
    average_satellites_visible /= ARRAY_SIZE;
    average_cog /= ARRAY_SIZE;
    average_vel /= ARRAY_SIZE;
    average_rssi /= ARRAY_SIZE;

};