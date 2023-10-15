#include "data.h"
#include "../src/config.h"

void process_data(telemetry_data *data, uint8_t calibration_flag, A4988 &stepper_motor, Servo &servo_motor){
    if(data->data_counter == (TELEMTRY_DATA_SAMPLES - 1)){

        data->alt_buffer += data->alt;
        data->relative_alt_buffer += data->relative_alt;
        data->drop_rate_comm_buffer += data->drop_rate_comm;
        data->lat_buffer += data->lat;
        data->lon_buffer += data->lon;
        data->satellites_visible_buffer += data->satellites_visible;
        data->fix_type_buffer += data->fix_type;
        data->rssi_buffer += data->rssi;

        data->average_alt = data->alt_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_relative_alt = data->relative_alt_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_drop_rate_comm = data->drop_rate_comm_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_lat = data->lat_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_lon = data->lon_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_satellites_visible = data->satellites_visible_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_fix_type = data->fix_type_buffer / TELEMTRY_DATA_SAMPLES;
        data->average_rssi = data->rssi_buffer / TELEMTRY_DATA_SAMPLES;

        data->alt_buffer = 0;
        data->relative_alt_buffer = 0;
        data->drop_rate_comm_buffer = 0;
        data->lat_buffer = 0;
        data->lon_buffer = 0;
        data->satellites_visible_buffer = 0;
        data->fix_type_buffer = 0;
        data->rssi_buffer = 0;

        data->data_counter = 0;

        if(calibration_flag == 1 && !KINEMATICS_DEBUG){
            stepper_motor.rotate(stepper_motor_logic(calculate_azimuth_deg(data->average_lat, data->average_lon, data->tracker_lat, data->tracker_lon), data->stepper_motor_postion));
            delay(500);
            servo_motor.write(calculate_elevation_deg(data->average_lat, data->average_lon, data->average_relative_alt, data->tracker_lat, data->tracker_lon));
            delay(500);
        }
    }
    else{
        
        data->alt_buffer += data->alt;
        data->relative_alt_buffer += data->relative_alt;
        data->drop_rate_comm_buffer += data->drop_rate_comm;
        data->lat_buffer += data->lat;
        data->lon_buffer += data->lon;
        data->satellites_visible_buffer += data->satellites_visible;
        data->fix_type_buffer += data->fix_type;
        data->rssi_buffer += data->rssi;

        data->data_counter++;
    }
};

void set_tracker_postion(telemetry_data *data){
    data->tracker_lat = data->average_lat;
    data->tracker_lon = data->average_lon;
};

