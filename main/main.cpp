#include "sdkconfig.h"
#include "Controller.hpp"
#include "console_register.h"
#include <driver/gpio.h>
#include "Encoder.hpp"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "Monitoring.hpp"
#include <stdio.h>
#include <string.h>

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);
#define WATCHDOG_PERIOD_MS 100
#define MAIN_TASK_PERIOD_MS 20
#define KB_STACK_SIZE 12
#define CORE_TAG "CORE_SYSTEM"

Controller controller;

static uint32_t main_tick = 0;
static uint8_t task_flag = 0;
bool option_change = false;

void initWatchdog() {
    // Initialize and start the watchdog timer
    //esp_task_wdt_init(5, true); // 5 seconds timeout, panic on timeout
    //esp_task_wdt_add(NULL); // Add current task to WDT watch
}

void runWatchdog() {
    // Reset the watchdog timer
    //esp_task_wdt_reset();
    vTaskDelay(WATCHDOG_PERIOD_MS / portTICK_PERIOD_MS);
}

void initSystem() {
    ESP_LOGI(CORE_TAG, "System initialization...");
    // Initialize all components
    Encoder::init();
    Monitoring::init();

#ifdef CONFIG_APP_CONSOLE_ENABLED
    init_console();
#endif

    ESP_LOGI(CORE_TAG, "System initialization done.");
}

/**
 * @brief Main state machine routine
 * 
 */
void mainRoutine(void *pvParameter) {
    // Main routine code here
    while (true) {

        switch (task_flag & 0xF0) {
            case 0x80: // ~25 Hz Monitoring
                Monitoring::readTemperature_probe1();
                Monitoring::readTemperature_probe2();
                Monitoring::readInternalTemperature();

                // reset the flag
                task_flag &= 0x7F;
                break;
            case 0x40: // ~10 Hz Controller
                controller.run();

                // reset the flag
                task_flag &= 0xBF;
                break;
            case 0x20: // ~1 Hz Cooker
                // cooker.update(); // Assuming cooker is a global or accessible object

                // reset the flag
                task_flag &= 0xDF;
                break;
            case 0x10: // ~0.2 Hz BME280
                // bme280.update(); // Assuming bme280 is a global or accessible object

                // reset the flag
                task_flag &= 0xEF;
                break;
            default:
                break;
        }

        // Set flags based on timing
        if ((main_tick % 2) == 0)  task_flag |= 0x80; // ~25 Hz si la boucle est à 50 Hz Monitoring
        if ((main_tick % 5) == 0)  task_flag |= 0x40; // ~10 Hz avec 50 Hz de tick Controller
        if ((main_tick % 50) == 0) task_flag |= 0x20; // ~1 Hz avec 50 Hz de tick Cooker
        if ((main_tick % 250) == 0) task_flag |= 0x10; // ~0.2 Hz avec 50 Hz de tick BME280

        main_tick++;

        vTaskDelay(MAIN_TASK_PERIOD_MS / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main() {
    initWatchdog();

    initSystem();

    // xTaskCreate(runWatchdog, "runWatchdog", 1024, NULL, 6, NULL);
    // xTaskCreate(mainRoutine, "mainRoutine", KB_STACK_SIZE * 1024, NULL, 4, NULL);

}