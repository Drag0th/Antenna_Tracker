#include "displayData.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "./modules/data_storage/data_storage.h"

SSD1306AsciiWire oled_display;

void printL(int32_t degE7) {
  // Extract and print negative sign
  if (degE7 < 0) {
    degE7 = -degE7;
    oled_display.print( '-' );
  }
  // Whole degrees
  int32_t deg = degE7 / 10000000L;
  oled_display.print( deg );
  oled_display.print( '.' );
  // Get fractional degrees
  degE7 -= deg*10000000L;
  // Print leading zeroes, if needed
  int32_t factor = 1000000L;
  while ((degE7 < factor) && (factor > 1L)){
    oled_display.print( '0' );
    factor /= 10L;
  }
  // Print fractional degrees
  oled_display.print( degE7 );
}

void display_data(telemetry_data *data){
  oled_display.clear();
  printL(data->lat_array[data->loop_counter]); //gps
  oled_display.print(" ");
  printL(data->lon_array[data->loop_counter]);
  oled_display.println();
  oled_display.println((String)"SATS: "+data->satellites_visible_array[data->loop_counter]+(String)" "+data->fix_type_array[data->loop_counter]+(String)"D "  "RSSI: "+map(data->rssi_array[data->loop_counter],0,255,0,100)+(String)"%");
  oled_display.println((String)"DR: "+data->drop_rate_comm_array[data->loop_counter]/100+(String)"%  "  "ALT: "+data->relative_alt_array[data->loop_counter]/1000+(String)"m");
  oled_display.println(data->current_battery_array[data->loop_counter]/100.0+(String)"A  "+data->voltage_battery_array[data->loop_counter]/1000.0+(String)"V  R:"+data->battery_remaining_array[data->loop_counter]+(String)"%");
}
