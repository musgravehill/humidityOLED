/*
  ardu_328p 8Mhz 3.3V
  dht22  AM2302
  OLED 128*64 i2c
*/
//#include <avr/pgmspace.h>

#include "LowPower.h"

#include <SPI.h>
#include "DHT.h"
DHT dht;

#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL);  //OLED SDA A4, SCL A5
extern uint8_t SmallFont[]; //6*8px
extern uint8_t MediumNumbers[]; //12*16px
extern uint8_t BigNumbers[]; //14*24px


//====================================== BUTTON ================================================
#define BUTTON_WAKEUP_PIN 2 //only d2 interrupt unSleep

//====================================== SYS =====================================================
#define BATT_CONTROL_PIN_1V1 A0 //hardcoded in PCB voltage divider
#define BATT_max 4.20
#define BATT_min 3.34
bool OLED_blinker_state = true;

//======================================= DHT humidity sensor =====================================
#define DHT22_DATA_PIN 3

void setup() {
  SYS_init();
  DHT_init();
  OLED_init();
}

void loop() {
  // Allow wake up pin to trigger interrupt on low.
  attachInterrupt(0, wakeUp, LOW); //D2
  // Enter power down state with ADC and BOD module disabled.
  // Wake up when wake up pin is low.
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); //43mkA
  // Disable external pin interrupt on wake up pin.
  detachInterrupt(0);
  //do staff here, if button activated
  OLED_display();
}

void wakeUp() {
}


