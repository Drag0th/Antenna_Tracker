#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <mavlink.h>
#include <Servo.h> 
#include "modules/data_storage/data_storage.h"
#include "modules/data_display/data_display.h"
#include "modules/kinematics/kinematics.h"
#include "modules/calibrations_and_tests/calibrations_and_tests.h"

//???
#define RST_PIN -1

//GIT
#define I2C_ADDRESS 0x3C
#define MAV_TIMEOUT 5000 //mavlink timeout
#define SERIAL_SPEED 57600 //mavlink input baud, 4800 for qczek 2.10

telemetry_data data_storage = {0};
AccelStepper stepper_motor(AccelStepper::DRIVER, STEPPER_DRIVER_DIR_PIN, STEPPER_DRIVER_DIR_PIN);
Servo servo_motor;
SSD1306AsciiWire oled;

uint8_t calibration_flag, data_flag, flag;

void set_flag();

mavlink_message_t msg;
mavlink_status_t status;


//------------------------------------------------------------------------------
void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  display_telemetry_lost(oled);
  Serial.begin(SERIAL_SPEED);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(BUTTION0_PIN, INPUT);
  servo_motor.attach(SERVO_MOTOR_PIN);
  //test_movement(stepper_motor, servo_motor);
}
//------------------------------------------------------------------------------
void loop() {
  while(Serial.available()) {
    uint8_t c= Serial.read();
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)){
      flag = 0;
      switch(msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: {
          break;
        }
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
          mavlink_global_position_int_t packet;
          mavlink_msg_global_position_int_decode(&msg, &packet);
          if(packet.hdg == 65535)
            packet.hdg = 0;
          if(data_storage.relative_alt_array[data_storage.loop_counter] != packet.relative_alt){
            data_storage.relative_alt_array[data_storage.loop_counter] = packet.relative_alt; 
            set_flag(); 
          }
          if(data_storage.hdg_array[data_storage.loop_counter] != packet.hdg){
            data_storage.hdg_array[data_storage.loop_counter] = packet.hdg;
            set_flag();
          }
          break;
        }
        case MAVLINK_MSG_ID_SYS_STATUS: {
          __mavlink_sys_status_t packet;
          mavlink_msg_sys_status_decode(&msg, &packet);
          if(data_storage.battery_remaining_array[data_storage.loop_counter] != packet.battery_remaining && packet.battery_remaining >= 0){
            data_storage.battery_remaining_array[data_storage.loop_counter] = packet.battery_remaining;
            set_flag();
          }
          if(data_storage.voltage_battery_array[data_storage.loop_counter] != packet.voltage_battery && packet.voltage_battery != 65535){
            data_storage.voltage_battery_array[data_storage.loop_counter] = packet.voltage_battery;
            set_flag();
          }
          if(data_storage.current_battery_array[data_storage.loop_counter] != packet.current_battery){
            data_storage.current_battery_array[data_storage.loop_counter] = packet.current_battery;
            set_flag();
          }
          if(data_storage.cpu_load_array[data_storage.loop_counter] != packet.load){
            data_storage.cpu_load_array[data_storage.loop_counter] = packet.load;
            set_flag();
          }
          if(data_storage.drop_rate_comm_array[data_storage.loop_counter] != packet.drop_rate_comm){
            data_storage.drop_rate_comm_array[data_storage.loop_counter] = packet.drop_rate_comm;
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
          if(data_storage.rssi_array[data_storage.loop_counter] != packet.rssi){
            data_storage.rssi_array[data_storage.loop_counter] = packet.rssi;
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
          if(data_storage.lat_array[data_storage.loop_counter] != packet.lat){
            data_storage.lat_array[data_storage.loop_counter] = packet.lat;
            set_flag();
          }
          if(data_storage.lon_array[data_storage.loop_counter] != packet.lon){
            data_storage.lon_array[data_storage.loop_counter] = packet.lon;
            set_flag();
          }
          if(data_storage.vel_array[data_storage.loop_counter] != packet.vel){
            data_storage.vel_array[data_storage.loop_counter] = packet.vel;
            set_flag();
          }
          if(data_storage.cog_array[data_storage.loop_counter] != packet.cog){
            data_storage.cog_array[data_storage.loop_counter] = packet.cog;
            set_flag();
          }
          if(data_storage.fix_type_array[data_storage.loop_counter] != packet.fix_type){
            data_storage.fix_type_array[data_storage.loop_counter] = packet.fix_type;
            set_flag();
          }
          if(data_storage.satellites_visible_array[data_storage.loop_counter] != packet.satellites_visible){
            data_storage.satellites_visible_array[data_storage.loop_counter] = packet.satellites_visible;
            set_flag();
          }
          break;
        }
        default:{
          #ifdef DEBUG
            Serial.println(msg.msgid); //see unused packet types
          #endif
          break;
        }
      }//switch
      if(flag == 1) {

        process_telemetry_data(&data_storage);

        
        
        if(calibration_flag == 1){
          //if(data_storage.average_relative_alt_backup != data_storage.average_relative_alt or data_storage.average_lat_backup != data_storage.average_lat or data_storage.average_lon_backup != data_storage.average_lon){
            //azimuth_movement(stepper_motor, calculate_azimuth_deg(data_storage.average_lat, data_storage.average_lon, data_storage.tracker_lat, data_storage.average_lon));
            //elevation_movement(servo_motor, calculate_elevation_deg(data_storage.average_lat, data_storage.average_lon, data_storage.average_relative_alt, data_storage.average_lat, data_storage.average_lon));
            //rssi_guard(&data_storage);
          //};
          if(data_flag == 0)
            display_data(&data_storage, oled);
          //else
            //display_average_data(&data_storage, oled);
        }
        else{
          display_calibration_message(oled);
        }
        
      }//print flag
      else {
        if(calibration_flag == 1){
          if(data_flag == 0)
            display_data(&data_storage, oled);
          //else
            //display_average_data(&data_storage, oled);
        }
        else{
          display_calibration_message(oled);
        }
      }
    }//if mavlink_parse_char
  }//while serial available

  if(calibration_flag == 0){
    display_calibration_message(oled);
    if((digitalRead(BUTTION0_PIN)) == HIGH){
      set_tracker_position(&data_storage);
      calibration_flag = 1;
    }
  }
  else{
    if((digitalRead(BUTTION0_PIN)) == HIGH){
      if(data_flag == 0){
        data_flag = 1;
      }
      else{
        data_flag = 0;
      }
    }
  }

  if(data_storage.loop_counter == (TELEMTRY_DATA_SAMPLES-1)){
    data_storage.loop_counter = 0;
  }
  else{
    data_storage.loop_counter++;
  }

}

void set_flag() {
    flag = 1;
}