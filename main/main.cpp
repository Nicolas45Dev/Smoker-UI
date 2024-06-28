#include "Controller.hpp"
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstdio>
#include <cstring>
#include "driver/ledc.h"

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);

#define SW_PIN (gpio_num_t)6
#define ENA_PIN (gpio_num_t)4
#define INB_PIN (gpio_num_t)5

Controller controller(100);

bool option_change = false;

void IRAM_ATTR rotaryEncoderInterrupt(void *arg) {
    // if ENB_PIN is high, then the direction is clockwise
    if(gpio_get_level(INB_PIN)) {
        controller.setOptionChange(true);
    } else {
        controller.setOptionChange(false);
    }
}

void IRAM_ATTR buttonInterrupt(void *arg) {
    controller.setPageChange();
}

void runController(void *pvParameter) {
    while(1) {
        controller.run();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief the system wiil be setup here before normal execution
 * 
 */
void setup() {
    // Set SW_PIN as input and internal on falling edge
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << SW_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(SW_PIN, buttonInterrupt, nullptr);

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

extern "C" void app_main() {
    setup();

    xTaskCreate(runController, "runController", 8192, nullptr, 4, nullptr);

    // Here is the task for the coooking controller
    //xTaskCreate(printDirection, "cooker", 4096, NULL, 5, NULL);
}