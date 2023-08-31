#include "kinematics.h"
#include "../src/config.h"

void check_motors(AccelStepper &stepper_motor, Servo &servo_motor){
    stepper_motor.moveTo(STEPS_PER_REVOLUTION);
    stepper_motor.setSpeed(STEPPER_MOTOR_SPEED);
    servo_motor.write(180);
    delay(1000);
    servo_motor.write(0);
    delay(1000);
};