#include "display.h"
#include "../config.h"

void display_current_data(telemetry_data *data, SSD1306AsciiWire &display) {
  display.clear();
  printL(data->lat, display); //gps
  display.print(" ");
  printL(data->lon, display);
  display.println();
  display.println((String)"SATS: "+data->satellites_visible+(String)" "+data->fix_type+(String)"D "  "RSSI: "+map(data->rssi,0,255,0,100)+(String)"%");
  display.println((String)"DR: "+data->drop_rate_comm/100+(String)"%  "  "ALT: "+data->relative_alt/1000+(String)"m");

  int rssi_alarm = map(data->rssi,0,255,0,100);
      if(rssi_alarm <= 30){
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      }
}

void printL(int32_t degE7, SSD1306AsciiWire &display) {
  // Extract and print negative sign
  if (degE7 < 0) {
    degE7 = -degE7;
    display.print( '-' );
  }
  // Whole degrees
  int32_t deg = degE7 / 10000000L;
  display.print( deg );
  display.print( '.' );
  // Get fractional degrees
  degE7 -= deg*10000000L;
  // Print leading zeroes, if needed
  int32_t factor = 1000000L;
  while ((degE7 < factor) && (factor > 1L)){
    display.print( '0' );
    factor /= 10L;
  }
  // Print fractional degrees
  display.print( degE7 );
}

void display_wait(SSD1306AsciiWire &display) {
  display.setFont(System5x7);
  display.set2X();
  display.clear();
  display.println(" TELEMETRY");
  display.println("    LOST   ");
  display.set1X();
  display.setFont(System5x7);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN,LOW);
}