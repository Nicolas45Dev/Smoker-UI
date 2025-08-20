#ifndef _ENCODER_ESP32_HAL_H
#define _ENCODER_ESP32_HAL_H

#include <driver/gpio.h>
#include <esp_attr.h>

static constexpr gpio_num_t SW_PIN = (gpio_num_t)CONFIG_ROTARY_ENCODER_PIN_BUTTON;
static constexpr gpio_num_t ENA_PIN = (gpio_num_t)CONFIG_ROTARY_ENCODER_PIN_A;
static constexpr gpio_num_t INB_PIN = (gpio_num_t)CONFIG_ROTARY_ENCODER_PIN_B;

namespace encoder_esp32_hal {

    IRAM_ATTR void rotaryEncoderInterrupt(void *arg);
    IRAM_ATTR void buttonInterrupt(void *arg);

    /**
     * @brief Initialize the encoder GPIO pins and interrupts
     * 
     */
    void init();

    /**
     * @brief Get the Option Change object
     * 
     * @return int8_t 
     */
    int8_t getOptionChange();

    /**
     * @brief Get the Page Change object
     * 
     * @return uint8_t 
     */
    uint8_t getPageChange();

    /**
     * @brief Check if there is an Option Change
     * 
     * @return true 
     * @return false 
     */
    bool isOptionChange();

    /**
     * @brief Check if there is a Page Change
     * 
     * @return true 
     * @return false 
     */
    bool isPageChange();
}

#endif