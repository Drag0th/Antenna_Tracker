#pragma once

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "./modules/data/data.cpp"

void display_current_data(telemetry_data *data, SSD1306AsciiWire &display);
void printL(int32_t degE7, SSD1306AsciiWire &display);
void display_wait(SSD1306AsciiWire &display);
