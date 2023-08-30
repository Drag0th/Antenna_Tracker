#include "kinematics.h"
#include "../src/config.h"

void check_motors(AccelStepper &stepper_motor, Servo &servo_motor){
    stepper_motor.moveTo(STEPS_PER_REVOLUTION);
    stepper_motor.setSpeed(STEPPER_MOTOR_SPEED);
    while(stepper_motor.distanceToGo() != 0) {
        stepper_motor.runSpeedToPosition();
    }
    stepper_motor.setCurrentPosition(0);
    servo_motor.write(180);
    servo_motor.write(0);
};