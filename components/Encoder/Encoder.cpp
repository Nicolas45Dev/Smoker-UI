#include "Encoder.hpp"

namespace Encoder {
    void init() {
        encoder_esp32_hal::init();
    }

    void process() {
        if (encoder_esp32_hal::isOptionChange()) {
            int8_t change = encoder_esp32_hal::getOptionChange();
            // Handle the option change, e.g., update a setting or UI element
        }

        if (encoder_esp32_hal::isPageChange()) {
            uint8_t change = encoder_esp32_hal::getPageChange();
            // Handle the page change, e.g., switch to a different screen
        }
    }
}

// The Encoder component can be initialized and processed in the main application loop
// For example, in the runController function, you can call Encoder::process() periodically
// to handle any encoder input changes. This allows the Encoder component to work seamlessly
// with the rest of the application logic.
