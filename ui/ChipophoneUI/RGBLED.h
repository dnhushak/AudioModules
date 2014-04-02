#include "Arduino.h"
#include "LED.h"

namespace ArduinoUI {
class RGBLED {
  public:
    RGBLED(int redPin, int grnPin, int bluPin);
    void setColor(unsigned char newRedIntensity, unsigned char newGrnIntensity, unsigned char newBluIntensity);
    void begin();
    void on();
    void off();
    ~RGBLED();
  private:
    ArduinoUI::LED * redLED;
    ArduinoUI::LED * grnLED;
    ArduinoUI::LED * bluLED;
};

}
