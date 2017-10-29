/*
  ardu_328p 8Mhz 3.3V
  dht22  AM2302
  OLED 128*64 i2c
*/
//#include <avr/pgmspace.h>

#include <SPI.h>
#include <stdint.h>
#include <avr/wdt.h>
#include "LowPower.h" //LP
#include "DHT.h"

//============================================ OLED ===========================================
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL);  //OLED SDA A4, SCL A5
extern uint8_t SmallFont[]; //6*8px
extern uint8_t MediumNumbers[]; //12*16px
extern uint8_t BigNumbers[]; //14*24px
bool OLED_blinker_state = true;

//======================================= SENSOR DATA ==========================================
int16_t humidity = 0;
int16_t temperature = 0;
float batteryVoltage = 0.0;

//====================================== BUTTON ================================================
#define BUTTON_WAKEUP_PIN 2 //only d2 interrupt unSleep

//====================================== SYS =====================================================
#define BATT_CONTROL_PIN_1V1 A0 //hardcoded in PCB voltage divider
#define BATT_max 4.20
#define BATT_min 3.34

//======================================= DHT humidity sensor =====================================
DHT dht;
#define DHT22_DATA_PIN 3

void setup() {
  SYS_init();
  DHT_init();
  OLED_init();
}

void loop() {
  attachInterrupt(0, wakeUp, LOW); //D2
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); //43mkA
  detachInterrupt(0);
  OLED_display();

  //reset base after 1 day uptime
  if ((int) millis() > 86400000L) {
    wdt_enable(WDTO_2S);
    delay(2500);
  }
}

void wakeUp() {

}


