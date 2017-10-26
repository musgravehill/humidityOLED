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

  pinMode(BUTTON_WAKEUP_PIN, INPUT_PULLUP);

  dht.setup(DHT22_DATA_PIN);  //delay(57000); //for PIR calibrating 60s

  myOLED.begin();
  myOLED.setBrightness(0);
  delay(20);
  myOLED.setOff();
  delay(20);
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
  float batteryVoltage = 0,005421 * analogRead(BATT_CONTROL_PIN_1V1);
  // 1M, 220K divider across battery and using internal ADC ref of 1.1V
  // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
  // ((1e6+220e3)/220e3)*1.1 = Vmax = 5,5454545454545454545454545454545 Volts
  // 5,5454545454545454545454545454545/1023 = Volts per bit = 0.003363075

  myOLED.setOn();
  delay(20);

  myOLED.clrScr();

  myOLED.setFont(SmallFont);
  myOLED.print("%", 55, 10);
  myOLED.print("o", 122, 0);
  myOLED.print("V", 55, 42);

  myOLED.setFont(BigNumbers);
  myOLED.printNumI(humidity, 0, 0);
  myOLED.printNumI(temperature, 66, 0);
  myOLED.printNumF(batteryVoltage, 2, 0, 32);

  myOLED.update();

  delay(7000); //21s display, then sleep
  myOLED.clrScr();
  myOLED.update();
  delay(20);
  myOLED.setOff();
  delay(20);
}

void wakeUp() {

}


