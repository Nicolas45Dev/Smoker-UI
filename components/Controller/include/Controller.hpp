#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <driver/gpio.h>
#include <esp_log.h>
#include <Model.hpp>
#include <stdio.h>
#include <View.hpp>
#include <stdint.h>
#include <BME280.hpp>
#include <MAX31855.hpp>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define THERMOMETER_UPDATE_INTERVAL 20
#define THERMOCOUPLE_UPDATE_INTERVAL 20
#define INACTIVE_TIMEOUT 20000
#define PAGE_COUNT 5

const uint8_t PAGE_OPTION_COUNT[PAGE_COUNT] = {0, 2, 4, 12, 5};

class Controller
{
private:
    uint8_t bme280_update_tick = 0;
    uint8_t thermocouple_update_tick = 0;
    uint32_t time_tick = 0;
    uint8_t sensor_size = 0;
    uint8_t page_option_max = 0;

    uint8_t page_option_history[PAGE_COUNT];

    bool page_change = false;
    bool is_active = true;
    int8_t option_change = 0;

    uint32_t previous_tick = 0;
    uint32_t current_tick = 0;
    uint32_t start_tick = 0;

    char thermo_tank[8];
    char thermo_meat1[8];
    char thermo_meat2[8];

    float bme280_data[3];

    View view;
    Model model;
    page_params_t page_params;

    void readModelData();
    void updateView();
    void updateModel();
    void setPageParams(bool withOption);

public:
    Controller(uint8_t update_interval);
    ~Controller();

    void run();
    void setPageChange();
    void setOptionChange(bool direction);
};

#endif // CONTROLLER_HPP
