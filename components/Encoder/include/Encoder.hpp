#ifndef _ENCODER_H
#define _ENCODER_H

#include "encoder_esp32_hal.hpp"

// The Encoder component provides an interface for handling rotary encoder inputs
// It uses the encoder_esp32_hal for hardware-specific operations

#include <cstdint>

// Forward declaration of the Encoder namespace
namespace Encoder {
    /**
     * @brief Initialize the encoder
     * 
     */
    void init();

    /**
     * @brief Run the main processing loop for the encoder
     * 
     */
    void process();
};

#endif