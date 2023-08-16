#include "kinematics.h"
#include "../config.h"
#include <AccelStepper.h>
#include <Servo.h>

void azimuth_movement(AccelStepper& stepper_motor, double deg){
    stepper_motor.moveTo(deg/(360/STEPS_PER_REVOLUTION));
    stepper_motor.setSpeed(STEPPER_MOTOR_SPEED);
    while(stepper_motor.distanceToGo() != 0) {
        stepper_motor.runSpeedToPosition();
    };
};

void elevation_movement(Servo& servo_motor, double deg){
    servo_motor.write(deg);
};