#include "encoder_esp32_hal.hpp"

static int8_t option_change = 0;
static uint8_t page_change = 0;

namespace encoder_esp32_hal {
    void init() {
        // Set SW_PIN as input and internal on falling edge
        gpio_config_t io_conf;
        io_conf.intr_type = GPIO_INTR_NEGEDGE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << SW_PIN);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        gpio_config(&io_conf);

        gpio_install_isr_service(0);
        gpio_isr_handler_add(SW_PIN, buttonInterrupt, NULL);

        // Set ENA_PIN as intput and interupt on falling edge
        io_conf.intr_type = GPIO_INTR_NEGEDGE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << ENA_PIN);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);

        // set INB_PIN as input
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_INPUT;
        io_conf.pin_bit_mask = (1ULL << INB_PIN);
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
        gpio_config(&io_conf);

        gpio_isr_handler_add(ENA_PIN, rotaryEncoderInterrupt, NULL);
    }

    IRAM_ATTR void rotaryEncoderInterrupt(void *arg) {
        // if ENB_PIN is high, then the direction is clockwise
        if(gpio_get_level(INB_PIN)) {
            option_change = 1;
        } else {
            option_change = -1;
        }
    }

    IRAM_ATTR void buttonInterrupt(void *arg) {
        page_change = 1;
    }

    uint8_t getPageChange() {
        uint8_t change = page_change;
        page_change = 0; // Reset after reading
        return change;
    }

    int8_t getOptionChange() {
        int8_t change = option_change;
        option_change = 0; // Reset after reading
        return change;
    }

    bool isOptionChange() {
        return option_change != 0;
    }

    bool isPageChange() {
        return page_change != 0;
    }
}