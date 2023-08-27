#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <mavlink.h>
#include <EEPROM.h>
//MODULES
#include "modules/data/data.h"

//MY VARIABLES
telemetry_data data_storage;
//

#define I2C_ADDRESS 0x3C
#define RST_PIN -1
#define buz 2
SSD1306AsciiWire oled;

#define MAV_TIMEOUT 5000 //mavlink timeout
#define SERIAL_SPEED 57600 //mavlink input baud, 4800 for qczek 2.10
//#define DEBUG
mavlink_message_t msg;
mavlink_status_t status;
//oth
uint8_t flag, eeprom_flag = 0;
uint32_t time_flag;

//DELETE
void display_wait();
void set_flag();
void printL(int32_t degE7);
void display_data();
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
    if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status)) {
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
          if(data_storage.relative_alt != packet.relative_alt){ 
            data_storage.relative_alt = packet.relative_alt; 
            set_flag(); 
          }
          if(data_storage.hdg != packet.hdg){
            data_storage.hdg = packet.hdg; 
            set_flag(); 
          }
          break;
        }
        case MAVLINK_MSG_ID_SYS_STATUS: {
          __mavlink_sys_status_t packet;
          mavlink_msg_sys_status_decode(&msg, &packet);
          if(data_storage.cpu_load != packet.load){
            data_storage.cpu_load = packet.load;
            set_flag();
          }
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
          if(data_storage.vel != packet.vel){ 
            data_storage.vel = packet.vel;
            set_flag();
          }
          if(data_storage.cog != packet.cog){
            data_storage.cog = packet.cog; 
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
          #ifdef DEBUG
          Serial.println(msg.msgid); //see unused packet types
          #endif
        break;
        }
    }//switch
    if(flag == 1) {
      display_data();
    }//print flag
    else {
      //no_data();
    }
   }//if mavlink_parse_char
  }//while serial available
  //no_data(); //check no serial input data fuction
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
  printL(data_storage.lat); //gps
  oled.print(" ");
  printL(data_storage.lon);
  oled.println();
  oled.println((String)"SATS: "+data_storage.satellites_visible+(String)" "+data_storage.fix_type+(String)"D "  "RSSI: "+map(data_storage.rssi,0,255,0,100)+(String)"%");
  oled.println((String)"DR: "+data_storage.drop_rate_comm/100+(String)"%  "  "ALT: "+data_storage.relative_alt/1000+(String)"m");

  int rssi_alarm = map(data_storage.rssi,0,255,0,100);
      if(rssi_alarm <= 30){
      digitalWrite(buz, HIGH);
      delay(500);
      digitalWrite(buz, LOW);
      }
}

void printL(int32_t degE7) {
  // Extract and print negative sign
  if (degE7 < 0) {
    degE7 = -degE7;
    oled.print( '-' );
  }
  // Whole degrees
  int32_t deg = degE7 / 10000000L;
  oled.print( deg );
  oled.print( '.' );
  // Get fractional degrees
  degE7 -= deg*10000000L;
  // Print leading zeroes, if needed
  int32_t factor = 1000000L;
  while ((degE7 < factor) && (factor > 1L)){
    oled.print( '0' );
    factor /= 10L;
  }
  // Print fractional degrees
  oled.print( degE7 );
}
