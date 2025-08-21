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
     * @brief Check if the option has changed
     * 
     * @return true 
     * @return false 
     */
    bool isOptionChanged();

    /**
     * @brief Check if the page has changed
     * 
     * @return true 
     * @return false 
     */
    bool isPageChanged();

    /**
     * @brief Get the Option Change object
     * 
     * @return true 
     * @return false 
     */
    bool getOptionChange();
};

#endif