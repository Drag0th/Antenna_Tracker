#include "data_storage.h"
#include <cmath>

#define pi 3.141592

//REWORK
void process_telemetry_data(telemetry_data *data){
    if(data->loop_counter == (TELEMTRY_DATA_SAMPLES-1)){

        data->average_relative_alt_backup = data->average_relative_alt;
        data->average_lat_backup = data->average_lat;
        data->average_lon_backup = data->average_lon;

        for(int i = 0 ; i < (TELEMTRY_DATA_SAMPLES-1) ; i++){
            data->average_relative_alt = data->average_relative_alt + data->relative_alt_array[i];
            data->average_hdg = data->average_hdg + data->hdg_array[i];
            data->average_battery_remaining = data->average_battery_remaining + data->battery_remaining_array[i];
            data->average_current_battery = data->average_current_battery + data->current_battery_array[i];
            data->average_voltage_battery = data->average_voltage_battery + data->voltage_battery_array[i];
            data->average_cpu_load = data->average_cpu_load + data->cpu_load_array[i];
            data->average_drop_rate_comm = data->average_drop_rate_comm + data->drop_rate_comm_array[i];
            data->average_lat = data->average_lat + data->lat_array[i];
            data->average_lon = data->average_lon + data->lon_array[i];
            data->average_satellites_visible = data->average_satellites_visible + data->satellites_visible_array[i];
            data->average_cog = data->average_cog + data->cog_array[i];
            data->average_vel = data->average_vel + data->vel_array[i];
            data->average_rssi = data->average_rssi + data->rssi_array[i];
        }

        data->average_relative_alt /= TELEMTRY_DATA_SAMPLES;
        data->average_hdg /= TELEMTRY_DATA_SAMPLES;
        data->average_battery_remaining /= TELEMTRY_DATA_SAMPLES;
        data->average_current_battery /= TELEMTRY_DATA_SAMPLES;
        data->average_voltage_battery /= TELEMTRY_DATA_SAMPLES;
        data->average_cpu_load /= TELEMTRY_DATA_SAMPLES;
        data->average_drop_rate_comm /= TELEMTRY_DATA_SAMPLES;
        data->average_lat /= TELEMTRY_DATA_SAMPLES;
        data->average_lon /= TELEMTRY_DATA_SAMPLES;
        data->average_satellites_visible /= TELEMTRY_DATA_SAMPLES;
        data->average_cog /= TELEMTRY_DATA_SAMPLES;
        data->average_vel /= TELEMTRY_DATA_SAMPLES;
        data->average_rssi /= TELEMTRY_DATA_SAMPLES;
        
    }
    else{
        
    }
};
//READY

double calculate_azimuth_deg(int32_t object_lat, int32_t object_lon, int32_t tracker_lat, int32_t tracker_lon){

    double object_lat_deg = convert_to_degrees(object_lat);
    double object_lon_deg = convert_to_degrees(object_lon);
    double tracker_lat_deg = convert_to_degrees(tracker_lat);
    double tracker_lon_deg = convert_to_degrees(tracker_lon);

    object_lat_deg -= tracker_lat_deg;
    object_lon_deg -= tracker_lon_deg;
    
    return(atan2(object_lat_deg,object_lon_deg)* (180 / pi));

};

double calculate_elevation_deg(int32_t object_lat, int32_t object_lon, int32_t object_alt, int32_t tracker_lat, int32_t tracker_lon){

    double object_lat_deg = convert_to_degrees(object_lat);
    double object_lon_deg = convert_to_degrees(object_lon);
    double tracker_lat_deg = convert_to_degrees(tracker_lat);
    double tracker_lon_deg = convert_to_degrees(tracker_lon);

    object_lat_deg -= tracker_lat_deg;
    object_lon_deg -= tracker_lon_deg;

    double distane_tracker_object = (sqrt(pow(object_lat_deg, 2)+pow(object_lon_deg, 2)))*DEG_TO_KM_RATIO;
    double distance_ground_object = ((object_alt/1000000) - TRIPOD_HEIGHT)/1000;

    return(asin(distance_ground_object/distane_tracker_object));
};

double convert_to_degrees(int32_t lat_or_lon ){
    int32_t whole_deg = lat_or_lon / 10000000L;
    double fractional_deg = (lat_or_lon - whole_deg*10000000L);
    while(fractional_deg > 1){
        fractional_deg /= 10;
    }
    return(whole_deg + fractional_deg);
};