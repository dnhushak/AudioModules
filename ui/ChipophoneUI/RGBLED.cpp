#include "RGBLED.h"

namespace UI {
RGBLED::RGBLED(int redPin, int grnPin, int bluPin) {
  redLED = new UI::LED(redPin);
  grnLED = new UI::LED(grnPin);
  bluLED = new UI::LED(bluPin);
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
