#pragma once

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "./modules/data/data.h"

void display_current_data(telemetry_data *data, SSD1306AsciiWire &display);
void printL(int32_t degE7, SSD1306AsciiWire &display);
void display_wait_message(SSD1306AsciiWire &display);
void display_calibration_message(SSD1306AsciiWire &display);
void display_no_data_message(telemetry_data *data, SSD1306AsciiWire &display);
void display_average_data(telemetry_data *data, SSD1306AsciiWire &display);
