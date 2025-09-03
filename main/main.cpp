#include "sdkconfig.h"
#include "Controller.hpp"
#include "console_register.h"
#include <driver/gpio.h>
#include "Encoder.hpp"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);

Controller controller;

bool option_change = false;

void runController(void *pvParameter) {
    if(!controller.is_init()) {
        controller.init();
    }
    for(;;) {
        controller.run();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main() {
#ifdef CONFIG_APP_CONSOLE_ENABLED
    init_console();
#endif

    // Initialize all components
    Encoder::init();

    xTaskCreate(runController, "runController", 6192, NULL, 4, NULL);
}