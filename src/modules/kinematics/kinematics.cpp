#include "kinematics.h"
#include "../src/config.h"

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