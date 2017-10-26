/*
  ardu_328p 8Mhz 3.3V
  dht22  d2
  OLED 128*64 i2c
*/

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
//#define DEBUG true

//======================================= DHT humidity sensor =====================================
#define DHT22_DATA_PIN 3

//===================================== OLED ====================================================
uint32_t OLED_time_start_ms = 0L;
bool OLED_isOn = false;

void setup() {
  delay(50);
  analogReference(INTERNAL); //1.1 V
  //use the 1.1 V internal reference => other A* can NOT receive VCC, only 1.1V max. But i2c a4a5 can works?
  /*
    DEFAULT: опорное напряжение по умолчанию, равное 5 В (на 5В-платах Ардуино) или 3.3 В (на 3.3В-платах Ардуино)
    INTERNAL: внутренне опорное напряжение, равное 1.1 В в микроконтроллерах ATmega168 и ATmega328, или 2.56 В в микроконтроллере ATmega8 (не доступно в Arduino Mega)
    INTERNAL1V1: внутреннее опорное напряжение 1.1 В (только для Arduino Mega)
    INTERNAL2V56: внутреннее опорное напряжение 2.56 В (только для Arduino Mega)
    EXTERNAL: в качестве опорного напряжения будет использоваться напряжение, приложенное к выводу AREF (от 0 до 5В)
  */

  delay(3000); //reduction of power fluctuation on powerUp

  pinMode(BUTTON_WAKEUP_PIN, INPUT);

  dht.setup(DHT22_DATA_PIN);  //delay(57000); //for PIR calibrating 60s

  myOLED.begin();
  myOLED.setBrightness(1);
}

void loop() {
  // Allow wake up pin to trigger interrupt on low.
  attachInterrupt(0, wakeUp, LOW); //D2

  // Enter power down state with ADC and BOD module disabled.
  // Wake up when wake up pin is low.
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  // Disable external pin interrupt on wake up pin.
  detachInterrupt(0);

  SYS_DISPLAY_INFO();
}

void SYS_DISPLAY_INFO() {
  //возможно, тут надо dht.setup после долгого сна
  int16_t humidity = (int) dht.getHumidity();
  int16_t temperature = (int) dht.getTemperature();
  int16_t batteryVoltage = 0.3334 * analogRead(BATT_CONTROL_PIN_1V1); // 100 * 3.24V = 324
  // 1M, 470K divider across battery and using internal ADC ref of 1.1V
  // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
  // ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
  // 3.44/1023 = Volts per bit = 0.003363075

  myOLED.clrScr();
  
  myOLED.setFont(SmallFont);
  myOLED.print("%", 28, 10);
  myOLED.setFont(BigNumbers);
  myOLED.printNumI(humidity, 0, 0);
  myOLED.printNumI(temperature, 34, 0);
  
  myOLED.setFont(MediumNumbers);
  myOLED.printNumF(batteryVoltage, 1, 74, 4);
  myOLED.setFont(SmallFont);
  myOLED.print("o", 122, 0);
   
  //uptime
  myOLED.setFont(SmallFont);
  myOLED.print("UPTIME DAYS", 0, 40);
  myOLED.printNumI(millis() / 86400000, 72, 40); 

  myOLED.update();

  delay(21000); //21s display, then sleep

}

void wakeUp() {

}


