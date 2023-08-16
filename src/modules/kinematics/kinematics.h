#pragma once
#include <AccelStepper.h>
#include <Servo.h>

void azimuth_movement(AccelStepper& stepper_motor, double deg);

void elevation_movement(Servo& servo_motor, double deg);