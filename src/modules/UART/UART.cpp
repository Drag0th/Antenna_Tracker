#include "UART.h"
#include <Arduino.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "Wire.h"

LiquidCrystal_I2C LCD(0x27,20,4);

void Initial_setup(){
    Serial.begin(9600);
    LCD.init();         
    LCD.backlight();
    LCD.clear();
    if (Serial.available() > 0) {
    }
    else{
        
    }
};