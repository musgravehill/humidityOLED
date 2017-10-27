void OLED_init() {
  myOLED.begin();
  myOLED.setBrightness(1);
  delay(20);
  myOLED.setOff(); //oled sleep, else it consumes 1.5mA. On pixels fire it uses 10mA
  delay(20);
}

void OLED_display() {
  int16_t humidity, temperature;
  float batteryVoltage;
  bool displayBlinker = false;

  delay(5);
  myOLED.setOn();
  delay(5);
  myOLED.clrScr();
  myOLED.setFont(SmallFont);
  myOLED.print(F("ANNA LOVE"), 20, 30);
  myOLED.update();

  //возможно, тут надо dht.setup после долгого сна
  //get first data, it may be damaged
  humidity = (int) dht.getHumidity();
  temperature = (int) dht.getTemperature();
  batteryVoltage = 0.00596285 * analogRead(BATT_CONTROL_PIN_1V1);
  delay(dht.getMinimumSamplingPeriod()); //dht22 need 2s pause between requesting data


  for (int i = 0; i < 5; i++) {
    humidity = (int) dht.getHumidity();
    temperature = (int) dht.getTemperature();
    batteryVoltage = 0.00596285 * analogRead(BATT_CONTROL_PIN_1V1);
    // 1M, 220K divider across battery and using internal ADC ref of 1.1V
    // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
    // ((1e6+220e3)/220e3)*1.1 = Vmax = 6.1000000 Volts
    // 6.1000/1023 = Volts per bit = 0.003363075

    myOLED.clrScr();
    myOLED.setFont(SmallFont);
    myOLED.print(F("%"), 55, 10);
    myOLED.print(F("o"), 122, 0);
    myOLED.print(F("V"), 55, 42);
    myOLED.setFont(BigNumbers);
    myOLED.printNumI(humidity, 0, 0);
    myOLED.printNumI(temperature, 66, 0);
    myOLED.printNumF(batteryVoltage, 2, 0, 32);
    OLED_render_batt(batteryVoltage);
    myOLED.update();

    delay(dht.getMinimumSamplingPeriod());//dht22 need 2s pause between requesting data
  }

  myOLED.clrScr();
  myOLED.update();
  delay(10);
  myOLED.setOff();
  delay(10);
}

void OLED_render_batt(float batteryVoltage) {
  if (batteryVoltage <= BATT_min) {
  }

  batteryVoltage = constrain(batteryVoltage, BATT_min,  BATT_max);
  int countLines = map(batteryVoltage, BATT_min, BATT_max, 0, 61);
  myOLED.drawRect(66, 32, 127, 48);
  for (int i = 0; i <= countLines; i++) {
    myOLED.drawLine(66 + i, 32, 66 + i, 48); 
  }

  myOLED.setFont(SmallFont);
  if (displayBlinker) {
    myOLED.print(F(" LOW BATT "), 66, 52);
    displayBlinker = false;
  } else {
    myOLED.print(F("*LOW BATT*"), 66, 52);
    displayBlinker = true;
  }


}




