#include "Arduino.h"
#include "LED.h"

namespace UI {
class RGBLED {
  public:
    RGBLED(int redPin, int grnPin, int bluPin);
    void setColor(unsigned char newRedIntensity, unsigned char newGrnIntensity, unsigned char newBluIntensity);
    void begin();
    void on();
    void off();
    ~RGBLED();
  private:
    UI::LED * redLED;
    UI::LED * grnLED;
    UI::LED * bluLED;
};

}
