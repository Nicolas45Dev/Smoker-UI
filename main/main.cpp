#include "sdkconfig.h"
#include "Cooker.hpp"
#include "Controller.hpp"
#include "console_register.h"
#include <driver/gpio.h>
#include "Encoder.hpp"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <GPIO.hpp>
#include <stdio.h>
#include <string.h>

#undef ESP_ERROR_CHECK
#define ESP_ERROR_CHECK(x)   do { esp_err_t rc = (x); if (rc != ESP_OK) { ESP_LOGE("err", "esp_err_t = %d", rc); assert(0 && #x);} } while(0);
#define WATCHDOG_PERIOD_MS 100
#define MAIN_TASK_PERIOD_MS 10
#define KB_STACK_SIZE 20
#define CORE_TAG "CORE_SYSTEM"

const gpio_num_t SANITY_LED_PIN = (gpio_num_t)17;

Controller controller;
Model* model;

static uint32_t main_tick = 0;
static uint8_t task_flag = 0;
bool option_change = false;

void initWatchdog() {
    gpio_set_direction(SANITY_LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(SANITY_LED_PIN, 0);
}

void runWatchdog(void *pvParameter) {
    // Toggle the sanity LED to indicate the system is alive
    while (true) {
        gpio_set_level(SANITY_LED_PIN, 1);
        vTaskDelay(WATCHDOG_PERIOD_MS / portTICK_PERIOD_MS);
        gpio_set_level(SANITY_LED_PIN, 0);
        vTaskDelay(WATCHDOG_PERIOD_MS / portTICK_PERIOD_MS);
    }
    vTaskDelay(WATCHDOG_PERIOD_MS / portTICK_PERIOD_MS);
}

void initSystem() {
    ESP_LOGI(CORE_TAG, "System initialization...");
    // Initialize all components
    Encoder::init();
    GPIO::init();
    model = Model::getInstance();
    controller.init();
    g_instance_cooker.init();

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

        if ((task_flag & 0x40) >> 6) { // ~10 Hz Controller
            controller.run();
            // reset the flag
            task_flag -= 0x40;
        }
        if ((task_flag & 0x80) >> 7) { // ~25 Hz Monitoring
            model->update();
            // reset the flag
            task_flag -= 0x80;
        }
        if ((task_flag & 0x20) >> 5) { // ~100 Hz Cooker
            g_instance_cooker.cooker_work();
            // reset the flag
            task_flag -= 0x20;
        }

        // Set flags based on timing
        if ((main_tick % 4) == 0)  task_flag += 0x80; // ~25 Hz si la boucle est à 50 Hz Monitoring
        if ((main_tick % 10) == 0)  task_flag += 0x40; // ~10 Hz avec 50 Hz de tick Controller
        if ((main_tick % 1) == 0) task_flag += 0x20; // ~100 Hz avec 50 Hz de tick Cooker

        main_tick++;

        vTaskDelay(MAIN_TASK_PERIOD_MS / portTICK_PERIOD_MS);
    }
}

extern "C" void app_main() {
    initWatchdog();

    initSystem();

    xTaskCreate(runWatchdog, "runWatchdog", 1024, NULL, 6, NULL);
    xTaskCreate(mainRoutine, "mainRoutine", KB_STACK_SIZE * 1024, NULL, 4, NULL);
}