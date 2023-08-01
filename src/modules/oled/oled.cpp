#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

SSD1306AsciiWire OLED;

void Initial_startup_text(){
    OLED.setFont(System5x7);
    OLED.set2X();
    OLED.clear();
    OLED.println(" TELEMETRY");
    OLED.println("    LOST   ");
    OLED.set1X();
    OLED.setFont(System5x7);
};