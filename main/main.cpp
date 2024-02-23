#include "Controller.hpp"
#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);

Controller controller(100);

// void IRAM_ATTR rotaryEncoderInterrupt(void *arg) {
    
// }

// void IRAM_ATTR buttonInterrupt(void *arg) {
    
// }

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
    //gpio_pad_select_gpio(ENCA_PIN);
    //gpio_set_direction(ENCA_PIN, GPIO_MODE_INPUT);

    //gpio_pad_select_gpio(ENCB_PIN);
    //gpio_set_direction(ENCB_PIN, GPIO_MODE_INPUT);

    //gpio_pad_select_gpio(BUTTON_PIN);
    //gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);

    // gpio_set_intr_type(ENCA_PIN, GPIO_INTR_NEGEDGE);
    // gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_NEGEDGE);

    // gpio_install_isr_service(0);
    // gpio_isr_handler_add(ENCA_PIN, rotaryEncoderInterrupt, (void *) ENCA_PIN);
    // gpio_isr_handler_add(BUTTON_PIN, buttonInterrupt, (void *) BUTTON_PIN);
}

extern "C" void app_main() {
    //setup();

    xTaskCreate(runController, "runController", 8192, NULL, 4, NULL);
}