#include "calibrations_and_tests.h"
#include "../config.h"
#include "./modules/data_storage/data_storage.h"

void test_movement(AccelStepper& stepper_motor, Servo& servo_motor){
    stepper_motor.moveTo(STEPS_PER_REVOLUTION);
    stepper_motor.setSpeed(STEPPER_MOTOR_SPEED);
    while(stepper_motor.distanceToGo() != 0) {
        stepper_motor.runSpeedToPosition();
    };
    servo_motor.write(0);
    servo_motor.write(180);
    servo_motor.write(0);
};

void set_tracker_position(telemetry_data *data){
    data->tracker_lat = data->average_lat;
    data->tracker_lon = data->average_lon;
};

void rssi_guard(telemetry_data *data){
    if(map(data->average_rssi,0,255,0,100) <= 30){
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
    }
};
