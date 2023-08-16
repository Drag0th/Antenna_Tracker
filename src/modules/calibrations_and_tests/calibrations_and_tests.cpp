#include "calibrations_and_tests.h"
#include "../config.h"

void test_movement(AccelStepper& stepper_motor, Servo& servo_motor){
    stepper_motor.moveTo(STEPS_PER_REVOLUTION);
    stepper_motor.setSpeed(STEPPER_MOTOR_SPEED);
    while(stepper_motor.distanceToGo() != 0) {
        stepper_motor.runSpeedToPosition();
    };
    servo_motor.write(0);
    servo_motor.write(180);
};