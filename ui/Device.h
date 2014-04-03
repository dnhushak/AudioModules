#ifndef DEVICE_HPP_
#define DEVICE_HPP_
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

#endif /* DEVICE_HPP_ */
