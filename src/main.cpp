#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <mavlink.h>
#include <EEPROM.h>
#include "modules/data_storage/data_storage.h"
#include "modules/displayData/displayData.h"


#define I2C_ADDRESS 0x3C
#define RST_PIN -1
#define buz 2
SSD1306AsciiWire oled;

#define MAV_TIMEOUT 5000 //mavlink timeout
#define SERIAL_SPEED 57600 //mavlink input baud, 4800 for qczek 2.10
//#define DEBUG
mavlink_message_t msg;
mavlink_status_t status;
//mavlink_global_position
int32_t alt, relative_alt;
int16_t vx, vy, vz;
uint16_t hdg;
//__mavlink_sys_status_t
int8_t battery_remaining;
uint16_t current_battery, voltage_battery, cpu_load, drop_rate_comm;
//mavlink_gps_raw_int_t
int32_t lat, lon, gps_alt;
uint8_t satellites_visible, fix_type;
uint16_t cog, vel;
//mavlink_rc_channels_raw_t
uint8_t rssi;
//oth
uint8_t flag, eeprom_flag = 0;
uint32_t time_flag;

//TEST
telemetry_data data_storage = {0};

void set_flag();
void display_wait();
void display_data();
void no_data();
int32_t EEPROM_int32_read(int addr);
void EEPROM_int32_write(int addr, int32_t data);
void printL(int32_t degE7);

//------------------------------------------------------------------------------
void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  display_wait();
  //
  Serial.begin(SERIAL_SPEED);
  //
  time_flag = millis();
  pinMode(buz, OUTPUT);
  digitalWrite(buz, LOW);
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
          if(lat != packet.lat){
            lat = packet.lat;
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

        //TEST
        process_telemetry_data(&data_storage);
        display_data();

        
      }//print flag
      else {
        no_data();
      }
    }//if mavlink_parse_char
  }//while serial available
  no_data(); //check no serial input data fuction
}

void set_flag() {
    flag = 1;
    eeprom_flag = 0;
    time_flag = millis();
}
void display_wait() {
  oled.setFont(System5x7);
  oled.set2X();
  oled.clear();
  //oled.println(" WAIT FOR");
  //oled.println("  MAVLINK");
  oled.println(" TELEMETRY");
  oled.println("    LOST   ");
  oled.set1X();
  oled.setFont(System5x7);

  digitalWrite(buz, HIGH);
  delay(100);
  digitalWrite(buz,LOW);
}

void display_data() {
  oled.clear();
  printL(lat); //gps
  oled.print(" ");
  printL(lon);
  oled.println();
  oled.println((String)"SATS: "+satellites_visible+(String)" "+fix_type+(String)"D "  "RSSI: "+map(rssi,0,255,0,100)+(String)"%");
  oled.println((String)"DR: "+drop_rate_comm/100+(String)"%  "  "ALT: "+relative_alt/1000+(String)"m");
  oled.println(current_battery/100.0+(String)"A  "+voltage_battery/1000.0+(String)"V  R:"+battery_remaining+(String)"%");

  int rssi_alarm = map(rssi,0,255,0,100);
      if(rssi_alarm <= 30){
      digitalWrite(buz, HIGH);
      delay(500);
      digitalWrite(buz, LOW);
      }
}

void no_data() {
  if((millis() - time_flag) > MAV_TIMEOUT ) { //no mavlink data at 2sec
     #ifdef DEBUG
     Serial.println((String)"LOST MAVLINK DATA");
     #endif
     display_wait();
     delay(300);
     if(eeprom_flag == 0 && lat != 0 && lon != 0 && fix_type > 1) { //if gps coordinates present, save it
        #ifdef DEBUG
        Serial.println("save..");
        #endif
        EEPROM_int32_write(5, lat);
        EEPROM_int32_write(16, lon);
        EEPROM.write(30, satellites_visible);
        EEPROM.write(32, fix_type);
        EEPROM.write(34, cpu_load);
        EEPROM.write(40, drop_rate_comm);
        EEPROM.write(46, hdg);
        EEPROM.write(52, vel);
        EEPROM_int32_write(56, relative_alt);
        EEPROM.write(66, current_battery);
        EEPROM.write(72, voltage_battery);
        EEPROM.write(74, battery_remaining);
        EEPROM.write(76, rssi);
        eeprom_flag = 1;
     } else { //no fresh data on mavlink, read from memory
        #ifdef DEBUG
        Serial.println("read..");
        #endif
        lat = EEPROM_int32_read(5);
        lon = EEPROM_int32_read(16);
        satellites_visible = EEPROM.read(30);
        fix_type = EEPROM.read(32);
        cpu_load = EEPROM.read(34);
        drop_rate_comm = EEPROM.read(40);
        hdg = EEPROM.read(46);
        vel = EEPROM.read(52);
        relative_alt = EEPROM_int32_read(56);
        current_battery = EEPROM.read(66);
        voltage_battery = EEPROM.read(72);
        battery_remaining = EEPROM.read(74);
        rssi = EEPROM.read(76); 
        eeprom_flag = 1;     
     }
     display_data();
     time_flag = millis();
  }
}

int32_t EEPROM_int32_read(int addr) // чтение из EEPROM 4 байта unsigned long
{   
  byte raw[4];
  for(byte i = 0; i < 4; i++) raw[i] = EEPROM.read(addr+i);
  int32_t &data = (int32_t&)raw;
  return data;
}
//*****************************************************************
void EEPROM_int32_write(int addr, int32_t data) // запись в EEPROM 4 байта unsigned long
{
  byte raw[4];
  (int32_t&)raw = data;
  for(byte i = 0; i < 4; i++) EEPROM.write(addr+i, raw[i]);
}
