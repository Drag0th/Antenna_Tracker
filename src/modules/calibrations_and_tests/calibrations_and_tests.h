#pragma once
#include <AccelStepper.h>
#include <Servo.h>
#include "./modules/data_storage/data_storage.h"

void test_movement(AccelStepper& stepper_motor, Servo& servo_motor);
void rssi_guard(telemetry_data *data);
