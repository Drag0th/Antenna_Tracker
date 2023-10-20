#include <unity.h>
#include "functions.h"

void setUp(void){}

void tearDown(void){}

void convert_to_degrees_test(){
    double actual = convert_to_degrees(6.8745783*pow(10,7));
    double expected = 6.8745783;
    TEST_ASSERT_EQUAL_DOUBLE(expected, actual);
}

void calculate_elevation_deg_test(){
    double acutal, expected, tracker_x, tracker_y, object_x, object_y, object_alt;
    object_x = 6*pow(10,7);
    object_y = 7*pow(10,7);
    object_alt = 22000;
    tracker_x = 3*pow(10,7);
    tracker_y = 3*pow(10,7);
    //###########
    expected = 0.00206471277;
    acutal = calculate_elevation_deg(object_x, object_y, object_alt, tracker_x, tracker_y);
    TEST_ASSERT_EQUAL_DOUBLE(expected, acutal);
}

void calculate_azimuth_deg_test(){
    double acutal, expected, tracker_x, tracker_y, object_x, object_y;
    object_x = 6*pow(10,7);
    object_y = 7*pow(10,7);
    tracker_x = 3*pow(10,7);
    tracker_y = 3*pow(10,7);
    expected = 53.1301024;
    acutal = calculate_azimuth_deg(object_x, object_y, tracker_x, tracker_y);
    TEST_ASSERT_EQUAL_DOUBLE(expected, acutal);
}

void stepper_motor_logic_test(){
    double current_postion, destination, acutal, expected;
    current_postion = 170.345678;
    destination = 190.567657;
    expected = 20.221979;
    acutal = stepper_motor_logic(destination, current_postion);
    TEST_ASSERT_EQUAL_DOUBLE(expected, acutal);
}

int main(int argc, char **argv){

    UNITY_BEGIN();
    RUN_TEST(convert_to_degrees_test);
    RUN_TEST(calculate_elevation_deg_test);
    RUN_TEST(calculate_azimuth_deg_test);
    RUN_TEST(stepper_motor_logic_test);
    UNITY_END();

    return 0;
}