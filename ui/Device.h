#ifndef DEVICE_H_
#define DEVICE_H_
#include <stdio.h>
#include <cstdlib>
#include "Arduino.h"

namespace synth {
enum devState_t {
  INACTIVE, ACTIVE
};

class Device {
  public:
    Device();

    // Return the state
    virtual devState_t getState();

    virtual ~Device();
  protected:
    devState_t state;
};

}

#endif /* DEVICE_H_ */
