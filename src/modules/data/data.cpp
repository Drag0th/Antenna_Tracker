#include "data.h"
#include "../src/config.h"

void process_data(telemetry_data *data){
    if(data->data_counter == (TELEMTRY_DATA_SAMPLES - 1)){

        //summarize_data(data);

        data->average_alt = data->alt_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_relative_alt = data->relative_alt_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_hdg = data->hdg_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_cpu_load = data->cpu_load_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_drop_rate_comm = data->drop_rate_comm_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_lat = data->lat_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_lon = data->lon_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_satellites_visible = data->satellites_visible_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_fix_type = data->fix_type_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_cog = data->cog_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_vel = data->vel_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_rssi = data->rssi_buffer / TELEMTRY_DATA_SAMPLES;

        data->alt_buffer = 0;
        data->relative_alt_buffer = 0;
        data->hdg_buffer = 0;
        data->cpu_load_buffer = 0;
        data->drop_rate_comm_buffer = 0;
        data->lat_buffer = 0;
        data->lon_buffer = 0;
        data->satellites_visible_buffer = 0;
        data->fix_type_buffer = 0;
        data->cog_buffer = 0;
        data->vel_buffer = 0;
        data->rssi_buffer = 0;

        data->data_counter = 0;
    }
    else{
        //summarize_data(data);

        data->data_counter++;
    }
};

void summarize_data(telemetry_data *data){
    data->alt_buffer += data->alt;
    data->relative_alt_buffer += data->relative_alt;
    data->hdg_buffer += data->hdg;
    data->cpu_load_buffer += data->cpu_load;
    data->drop_rate_comm_buffer += data->drop_rate_comm;
    data->lat_buffer += data->lat;
    data->lon_buffer += data->lon;
    data->satellites_visible_buffer += data->satellites_visible;
    data->fix_type_buffer += data->fix_type;
    data->cog_buffer += data->cog;
    data->vel_buffer += data->vel;
    data->rssi_buffer += data->rssi;
};


void set_tracker_postion(telemetry_data *data){
    data->tracker_lat = data->average_lat;
    data->tracker_lon = data->average_lon;
};

