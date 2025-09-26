#include "Encoder.hpp"

static int8_t change = 0;
static uint8_t page = 0;

namespace Encoder {
    void init() {
        encoder_esp32_hal::init();
    }

    bool isOptionChanged() {
        return encoder_esp32_hal::isOptionChange();
    }

    bool isPageChanged() {
        bool is_page = encoder_esp32_hal::isPageChange();
        encoder_esp32_hal::getPageChange();
        return is_page;
    }

    bool getOptionChange() {
        return encoder_esp32_hal::getOptionChange() == 1;
    }
}

// The Encoder component can be initialized and processed in the main application loop
// For example, in the runController function, you can call Encoder::process() periodically
// to handle any encoder input changes. This allows the Encoder component to work seamlessly
// with the rest of the application logic.