#include "kinematics.h"
#include "../src/config.h"

#define PI 3.1415

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

float stepper_motor_logic(float input_degrees, uint16_t &current_postion){

    float return_buffer;

    if(current_postion > input_degrees){
        if(input_degrees > 180){
            return_buffer = current_postion - input_degrees;
            current_postion = input_degrees;
            return return_buffer;
        }
        else{
            return_buffer = ((360 - current_postion) + input_degrees);
            current_postion = input_degrees;
            return return_buffer;
        }
    }
    else if(current_postion < input_degrees){
        if((input_degrees - current_postion) > 180){
            return_buffer = (-180 + ((input_degrees - current_postion) - 180));
            current_postion = input_degrees;
            return return_buffer;
        }
        else{
            return_buffer = input_degrees - current_postion;
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
    
    return(atan2(object_lat_deg,object_lon_deg) * (180 / PI));

};

double calculate_elevation_deg(int32_t object_lat, int32_t object_lon, int32_t object_alt, int32_t tracker_lat, int32_t tracker_lon){

    double object_lat_deg = convert_to_degrees(object_lat);
    double object_lon_deg = convert_to_degrees(object_lon);
    double tracker_lat_deg = convert_to_degrees(tracker_lat);
    double tracker_lon_deg = convert_to_degrees(tracker_lon);

    object_lat_deg -= tracker_lat_deg;
    object_lon_deg -= tracker_lon_deg;

    double distane_tracker_object = (sqrt(pow(object_lat_deg, 2)+pow(object_lon_deg, 2))) * DEG_TO_KM_RATIO;
    double distance_ground_object = ((object_alt/1000000) - TRIPOD_HEIGHT)/1000;

    return(asin(distance_ground_object/distane_tracker_object) * (180 / PI));
};

double convert_to_degrees(int32_t lat_or_lon ){
    int32_t whole_deg = lat_or_lon / 10000000L;
    double fractional_deg = (lat_or_lon - whole_deg*10000000L);
    while(fractional_deg > 1){
        fractional_deg /= 10;
    }
    return(whole_deg + fractional_deg);
};