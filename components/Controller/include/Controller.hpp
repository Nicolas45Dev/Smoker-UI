#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Cooker.hpp"
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

#define COOKER_INTERVAL 500
#define THERMOCOUPLE_UPDATE_INTERVAL 5000
#define INACTIVE_TIMEOUT 20000
#define PAGE_COUNT 5

const uint8_t PAGE_OPTION_COUNT[PAGE_COUNT] = {0, 2, 4, 12, 5};

class Controller
{
private:
    uint32_t cooker_update_tick = 0;
    uint32_t thermocouple_update_tick = 0;
    uint32_t time_tick = 0;

    uint8_t sensor_size = 0;
    uint8_t page_option_max = 0;
    uint8_t page_index = 0;

    bool is_active = true;
    bool starting = true;
    bool m_is_init = false;

    int8_t option_change = 0;

    uint32_t previous_tick = 0;
    uint32_t current_tick = 0;
    uint32_t start_tick = 0;

    uint32_t time_meat_1 = 0;
    uint32_t time_meat_2 = 0;

    char thermo_tank[8];
    char thermo_meat1[8];
    char thermo_meat2[8];
    char thermo_meat1_set[8];
    char thermo_meat2_set[8];
    
    float previous_thermo_1;
    float previous_thermo_2;

    View view;
    Model* model;
    Cooker cooker;
    page_params_t page_params;

    void updateView();
    void updateModel();
    void setPageParams(bool withOption);
    void setMenuPageFromOption();
    void setMeatProfilePageFromOption();
    void readThermocouples();

public:
    Controller(uint8_t update_interval);
    ~Controller();

    void run();
    bool init();
    bool is_init() { return m_is_init; }
    void setPageChange();
    void setOptionChange(bool direction);
};

#endif // CONTROLLER_HPP
