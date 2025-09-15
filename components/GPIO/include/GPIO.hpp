#ifndef GPIO_HPP
#define GPIO_HPP

#include "Config.h"
#include "sdkconfig.h"
#include "MCP23017.hpp"
#include <stdint.h>

namespace GPIO {

    static MCP23017 io_expander;

    void init();
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t value);
    int digitalRead(uint8_t pin);
};

#endif // GPIO_HPP