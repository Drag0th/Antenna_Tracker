#include <Wire.h>
#include <mavlink.h>
//MODULES
#include "config.h"
#include "modules/display/display.h"
#include "modules/kinematics/kinematics.h"
#include "modules/data/data.h" 

//MY VARIABLES
telemetry_data data_storage;
SSD1306AsciiWire oled_display;
A4988 stepper_motor(STEPS_PER_REVOLUTION, STEPPER_DRIVER_DIR_PIN, STEPPER_DRIVER_STEP_PIN);
Servo servo_motor;
//

#define I2C_ADDRESS 0x3C

#define MAV_TIMEOUT 5000 //mavlink timeout
#define SERIAL_SPEED 57600 //mavlink input baud, 4800 for qczek 2.10
//#define DEBUG
mavlink_message_t msg;
mavlink_status_t status;
//oth
uint8_t flag, calibration_flag, message_switch, display_flag;
uint32_t time_flag, calibration_time_flag, calibration_time_check;

//DELETE
void set_flag();
//------------------------------------------------------------------------------
void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  oled_display.begin(&Adafruit128x32, I2C_ADDRESS);
  Serial.begin(SERIAL_SPEED);
  time_flag = millis();
  //buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  //button0
  pinMode(BUTTION0_PIN, INPUT);
  //servo
  servo_motor.attach(SERVO_MOTOR_PIN, 544, 2400);
  //stepper
  stepper_motor.setSpeedProfile(A4988::LINEAR_SPEED, STEPPER_MOTOR_ACCELERATION, STEPPER_MOTOR_DECELERATION);
  stepper_motor.begin(STEPPER_MOTOR_RPM, STEPPER_MOTOR_MICROSTEPS);
  //movement check
  check_motors(stepper_motor, servo_motor);
}
//------------------------------------------------------------------------------
void loop() {
  while(Serial.available()) {
    uint8_t c= Serial.read();
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
    flag = 0;
    switch(msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: {
          break;
        }
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
          mavlink_global_position_int_t packet;
          mavlink_msg_global_position_int_decode(&msg, &packet);
          if(data_storage.relative_alt != packet.relative_alt){ 
            data_storage.relative_alt = packet.relative_alt; 
            set_flag(); 
          }
          break;
        }
        case MAVLINK_MSG_ID_SYS_STATUS: {
          __mavlink_sys_status_t packet;
          mavlink_msg_sys_status_decode(&msg, &packet);
          if(data_storage.drop_rate_comm != packet.drop_rate_comm){
            data_storage.drop_rate_comm = packet.drop_rate_comm;
            set_flag();
          }
          break;
        }
        case MAVLINK_MSG_ID_ATTITUDE: {
          break;
        }
        case MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN: {
          break;
        }
        case MAVLINK_MSG_ID_RC_CHANNELS_RAW: {
          __mavlink_rc_channels_raw_t packet;
          mavlink_msg_rc_channels_raw_decode(&msg, &packet);
          if(data_storage.rssi != packet.rssi){
            data_storage.rssi = packet.rssi;
            set_flag();
          }
          break;
        }
        case MAVLINK_MSG_ID_VFR_HUD: {
          break;
        }
        case MAVLINK_MSG_ID_GPS_RAW_INT: {
          __mavlink_gps_raw_int_t packet;
          mavlink_msg_gps_raw_int_decode(&msg, &packet);
          if(packet.cog == 65535)
            packet.cog = 0;
          if(packet.vel == 65535)
            packet.vel = 0;
          if(packet.alt == 65535)
            packet.alt = 0;
          if(data_storage.lat != packet.lat){
            data_storage.lat = packet.lat;
            set_flag();
          }
          if(data_storage.lon != packet.lon){
            data_storage.lon = packet.lon;
            set_flag();
          }
          if(data_storage.fix_type != packet.fix_type){
            data_storage.fix_type = packet.fix_type;
            set_flag(); 
          }
          if(data_storage.satellites_visible != packet.satellites_visible){
            data_storage.satellites_visible = packet.satellites_visible;
            set_flag();
          }
          break;
        }
        default: {
        break;
        }
    }
    if(flag == 1) {

      process_data(&data_storage, calibration_flag, stepper_motor, servo_motor);

      calibration_time_check = millis();

      if(calibration_flag == 0){
        if (calibration_time_check - calibration_time_flag >= 100UL) {
          calibration_time_flag = calibration_time_check;
          if(message_switch == 0){
            display_calibration_message(oled_display);
            message_switch = 1;
          }
          else{
            display_current_data(&data_storage, oled_display);
            message_switch = 0;
          }
        }
      }
      else
        display_data(display_flag, &data_storage, oled_display);
    }
    else {
      if((millis() - time_flag) > MAV_TIMEOUT )
        display_no_data_message(&data_storage, oled_display);
    }
   }
  }
  if((millis() - time_flag) > MAV_TIMEOUT ) 
    display_no_data_message(&data_storage, oled_display);
    
  //
  if(digitalRead(BUTTION0_PIN) == HIGH){
    if(calibration_flag != 1){
      set_tracker_postion(&data_storage);
      calibration_flag = 1;
    }
    else{
      display_shift(display_flag);
    }
  }
  // 
}

void set_flag() {
    flag = 1;
    time_flag = millis();
}

