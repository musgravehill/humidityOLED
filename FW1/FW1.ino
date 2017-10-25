/*
  ardu_328
  dht22  d2
*/


#include <SPI.h>
#include <stdint.h>
#include "LowPower.h"
#include "DHT.h"
#include <avr/wdt.h>

#define BATT_CONTROL_PIN_1V1 A0 //hardcoded in PCB voltage divider
#define DHT22_DATA_PIN 2

DHT dht;

void setup() {
  MCUSR = 0;  //VERY VERY IMPORTANT!!!! ELSE WDT DOESNOT RESET, DOESNOT DISABLED!!!
  wdt_disable();
  delay(50);
  analogReference(INTERNAL); //1.1 V

  delay(3000); //reduction of power fluctuation on powerUp

  Serial.begin(9600);

  dht.setup(DHT22_DATA_PIN);
  delay(57000); //for PIR calibrating 60s

  //use the 1.1 V internal reference => other A* can NOT receive VCC, only 1.1V max
  /*
    DEFAULT: опорное напряжение по умолчанию, равное 5 В (на 5В-платах Ардуино) или 3.3 В (на 3.3В-платах Ардуино)
    INTERNAL: внутренне опорное напряжение, равное 1.1 В в микроконтроллерах ATmega168 и ATmega328, или 2.56 В в микроконтроллере ATmega8 (не доступно в Arduino Mega)
    INTERNAL1V1: внутреннее опорное напряжение 1.1 В (только для Arduino Mega)
    INTERNAL2V56: внутреннее опорное напряжение 2.56 В (только для Arduino Mega)
    EXTERNAL: в качестве опорного напряжения будет использоваться напряжение, приложенное к выводу AREF (от 0 до 5В)
  */

  

}

void loop() {
  wdt_enable (WDTO_8S);
  //try to have time < 8s, else autoreset by watchdog
  //----DO STUFF HERE
  todo();


  wdt_reset();
  wdt_disable();

  //Serial.flush(); //the system is going to sleep while it's still sending the serial data.
  uint8_t countSleep = 0;
  while (countSleep < 2) {
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
    countSleep++;
  }

  //reset
  //wdt_enable(WDTO_2S);
  //delay(2500);

}

void todo() {
  dht.setup(DHT22_DATA_PIN);
  delay(100);
  uint16_t humidity = (int) dht.getHumidity();
  delay(20);
  uint16_t temperature = (int) dht.getTemperature();
  delay(20);
  uint16_t batteryVoltage = 0.3334 * analogRead(BATT_CONTROL_PIN_1V1); // 100 * 3.24V = 324


  // 1M, 470K divider across battery and using internal ADC ref of 1.1V
  // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
  // ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
  // 3.44/1023 = Volts per bit = 0.003363075


  Serial.print(humidity);
  Serial.print("% \t t=");
  Serial.print(temperature);
  Serial.println(" ");

}


