#include "GPIO.hpp"

namespace GPIO {
    void init() {
        // Initialize GPIO system if needed
        io_expander.begin();

        // Setup pins
        io_expander.pinMode(MCP23017Pins::RELAY1_PIN, 1); // Output
        io_expander.pinMode(MCP23017Pins::RELAY2_PIN, 1); // Output
        io_expander.pinMode(MCP23017Pins::PROBE_DETECT1_PIN, 0); // Input
        io_expander.pinMode(MCP23017Pins::PROBE_DETECT2_PIN, 0); // Input
        io_expander.pinMode(MCP23017Pins::PROBE_DETECT3_PIN, 0); // Input
        io_expander.pinMode(MCP23017Pins::BUZZER_PIN, 1); // Output
        io_expander.pinMode(MCP23017Pins::EN5V_PIN, 1); // Output
        io_expander.pinMode(MCP23017Pins::EN_MOTOR_PIN, 1); // Output
    }

    void pinMode(uint8_t pin, uint8_t mode) {
        // if the pin is an ESP32 pin use gpio functions else use the MCP23017
        if (pin <= 39) {
            gpio_set_direction((gpio_num_t)pin, (mode == 0) ? GPIO_MODE_INPUT : GPIO_MODE_OUTPUT);
        }
        else {
            io_expander.pinMode(pin, mode);
        }
    }

    void digitalWrite(uint8_t pin, uint8_t value) {
        if (pin <= 39) {
            gpio_set_level((gpio_num_t)pin, value);
        }
        else {
            io_expander.digitalWrite(pin, value);
        }
    }

    int digitalRead(uint8_t pin) {
        if (pin <= 39) {
            return gpio_get_level((gpio_num_t)pin);
        }
        else {
            return io_expander.digitalRead(pin);
        }
    }
};