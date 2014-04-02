#include "RGBLED.h"

namespace ArduinoUI {
RGBLED::RGBLED(int redPin, int grnPin, int bluPin) {
  redLED = new ArduinoUI::LED(redPin);
  grnLED = new ArduinoUI::LED(grnPin);
  bluLED = new ArduinoUI::LED(bluPin);
}
void RGBLED::setColor(unsigned char newRedIntensity, unsigned char newGrnIntensity, unsigned char newBluIntensity) {
  redLED->setIntensity(newRedIntensity);
  grnLED->setIntensity(newGrnIntensity);
  bluLED->setIntensity(newBluIntensity);
}
void RGBLED::begin() {
  redLED->begin();
  grnLED->begin();
  bluLED->begin();
}
void RGBLED::on() {
  redLED->on();
  grnLED->on();
  bluLED->on();
}

void RGBLED::off(){
  redLED->on();
  grnLED->on();
  bluLED->on();
}
}
