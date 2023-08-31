#include "kinematics.h"
#include "../src/config.h"

void check_motors(A4988 &stepper_motor, Servo &servo_motor){
    digitalWrite(BUZZER_PIN, HIGH); //daje znać że żyje 
    delay(100);
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