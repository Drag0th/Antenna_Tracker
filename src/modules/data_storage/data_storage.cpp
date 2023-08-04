#include "data_storage.h"

void process_telemetry_data(telemetry_data *data){
    if(data->loop_counter < 9){
        for(int i = 0 ; i < (ARRAY_SIZE-1) ; i++){
            data->average_relative_alt = data->average_relative_alt + data->relative_alt_array[data->loop_counter];
            data->average_hdg = data->average_hdg + data->hdg_array[data->loop_counter];
            data->average_battery_remaining = data->average_battery_remaining + data->battery_remaining_array[data->loop_counter];
            data->average_current_battery = data->average_current_battery + data->current_battery_array[data->loop_counter];
            data->average_voltage_battery = data->average_voltage_battery + data->voltage_battery_array[data->loop_counter];
            data->average_cpu_load = data->average_cpu_load + data->cpu_load_array[data->loop_counter];
            data->average_drop_rate_comm = data->average_drop_rate_comm + data->drop_rate_comm_array[data->loop_counter];
            data->average_lat = data->average_lat + data->lat_array[data->loop_counter];
            data->average_lon = data->average_lon + data->lon_array[data->loop_counter];
            data->average_satellites_visible = data->average_satellites_visible + data->satellites_visible_array[data->loop_counter];
            data->average_cog = data->average_cog + data->cog_array[data->loop_counter];
            data->average_vel = data->average_vel + data->vel_array[data->loop_counter];
            data->average_rssi = data->average_rssi + data->rssi_array[data->loop_counter];
        }

        data->loop_counter++;
        
    }
    else{
        data->average_relative_alt /= ARRAY_SIZE;
        data->average_hdg /= ARRAY_SIZE;
        data->average_battery_remaining /= ARRAY_SIZE;
        data->average_current_battery /= ARRAY_SIZE;
        data->average_voltage_battery /= ARRAY_SIZE;
        data->average_cpu_load /= ARRAY_SIZE;
        data->average_drop_rate_comm /= ARRAY_SIZE;
        data->average_lat /= ARRAY_SIZE;
        data->average_lon /= ARRAY_SIZE;
        data->average_satellites_visible /= ARRAY_SIZE;
        data->average_cog /= ARRAY_SIZE;
        data->average_vel /= ARRAY_SIZE;
        data->average_rssi /= ARRAY_SIZE;

        data->loop_counter = 0;
    }
};