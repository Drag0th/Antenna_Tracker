#pragma once
#include <Arduino.h>
#include "./modules/data_storage/data_storage.h"

void printL(int32_t degE7, SSD1306AsciiWire &oled_display);
void display_data(telemetry_data *data, SSD1306AsciiWire &oled_display);
void display_average_data(telemetry_data *data, SSD1306AsciiWire &oled_display);
void display_calibration_message(SSD1306AsciiWire &oled_display);
void display_telemetry_lost(SSD1306AsciiWire &oled_display);