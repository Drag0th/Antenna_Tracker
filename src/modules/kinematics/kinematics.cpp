#include "kinematics.h"
#include "../src/config.h"

#define PI 3.14159265359

void check_motors(A4988 &stepper_motor, Servo &servo_motor){
    digitalWrite(BUZZER_PIN, HIGH); //daje znać że żyje 
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(5000); //zwłoka 5s żeby po podłączeniu zasilania nie zajebało mi po łapach anteną 
    stepper_motor.rotate(360);
    delay(100);
    stepper_motor.rotate(-360);
    servo_motor.write(90);
    delay(1000);
    servo_motor.write(0);
    delay(1000);
    servo_motor.write(90);
};

double stepper_motor_logic(double input_degrees, double current_postion){

    double return_buffer;

    if(current_postion > input_degrees){
        if((current_postion - input_degrees) < 180){
            return_buffer = input_degrees - current_postion;
            current_postion = input_degrees;
            return return_buffer;
        }
        else{
            return_buffer = (360 - current_postion) + input_degrees;
            current_postion = input_degrees;
            return return_buffer;
        }
    }
    else if(current_postion < input_degrees){
        if((input_degrees - current_postion) < 180){
            return_buffer = input_degrees - current_postion;
            current_postion = input_degrees;
            return return_buffer;
        }
        else{
            return_buffer = (input_degrees - 360) - current_postion;
            current_postion = input_degrees;
            return return_buffer;
        }
    }
    else
        return 0;
};

double calculate_azimuth_deg(int32_t object_lat, int32_t object_lon, int32_t tracker_lat, int32_t tracker_lon){

    double object_lat_deg = convert_to_degrees(object_lat);
    double object_lon_deg = convert_to_degrees(object_lon);
    double tracker_lat_deg = convert_to_degrees(tracker_lat);
    double tracker_lon_deg = convert_to_degrees(tracker_lon);

    object_lat_deg -= tracker_lat_deg;
    object_lon_deg -= tracker_lon_deg;
    
    return(atan2(object_lon_deg, object_lat_deg) * (180 / PI));

};

double calculate_elevation_deg(int32_t object_lat, int32_t object_lon, int32_t object_alt, int32_t tracker_lat, int32_t tracker_lon){

    double object_lat_deg = convert_to_degrees(object_lat);
    double object_lon_deg = convert_to_degrees(object_lon);
    double tracker_lat_deg = convert_to_degrees(tracker_lat);
    double tracker_lon_deg = convert_to_degrees(tracker_lon);

    object_lat_deg -= tracker_lat_deg;
    object_lon_deg -= tracker_lon_deg;

    double distance_tracker_object = (sqrt(pow(object_lat_deg, 2) + pow(object_lon_deg, 2))) * DEG_TO_KM_RATIO;
    double distance_ground_object = (((double)object_alt/1000) - TRIPOD_HEIGHT) / 1000;
 
    return(atan(distance_ground_object/distance_tracker_object) * (180 / PI));
};

double convert_to_degrees(int32_t lat_or_lon ){
    return((double)lat_or_lon / (double)10000000);
};