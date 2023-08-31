#include "kinematics.h"
#include "../src/config.h"

void check_motors(A4988 &stepper_motor, Servo &servo_motor){
    stepper_motor.rotate(360);
    servo_motor.write(180);
    delay(1000);
    servo_motor.write(0);
    delay(1000);
};