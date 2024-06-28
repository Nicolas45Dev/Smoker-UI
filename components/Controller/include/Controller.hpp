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

#define THERMOMETER_UPDATE_INTERVAL 10000
#define THERMOCOUPLE_UPDATE_INTERVAL 5000
#define INACTIVE_TIMEOUT 20000
#define PAGE_COUNT 5

const uint8_t PAGE_OPTION_COUNT[PAGE_COUNT] = {0, 2, 4, 11, 3};

class Controller
{
private:

    uint8_t sensor_size = 0;
    uint8_t page_option_max = 0;
    uint8_t page_index = 0;

    int8_t option_change = 0;

    char thermo_tank[8];
    char thermo_meat1[8];
    char thermo_meat2[8];
    char thermo_meat1_set[8];
    char thermo_meat2_set[8];
    char bme280_data_str_temp[8];
    char bme280_data_str_hum[8];

    uint32_t bme280_update_tick = 0;
    uint32_t thermocouple_update_tick = 0;
    uint32_t time_tick = 0;
    uint32_t previous_tick = 0;
    uint32_t current_tick = 0;
    uint32_t start_tick = 0;
    uint32_t time_meat_1 = 0;
    uint32_t time_meat_2 = 0;

    bool is_active = true;
    bool starting = true;
    
    float previous_thermo_1;
    float previous_thermo_2;

    View view;
    Model model;
    page_params_t page_params{};

    /**
     * @brief Read the model data
     */
    void readModelData();

    /**
     * @brief Update the view every update interval
     */
    void updateView();

    /**
     * @brief Set the page parameters
     * @param withOption The flag indicating if the page has options
     */
    void setPageParams(const bool withOption);

    /**
     * @brief Set the menu page from option
     */
    void setMenuPageFromOption();

    /**
     * @brief Set the meat profile page from option
     */
    void setMeatProfilePageFromOption();

    /**
     * @brief Set the setting page from option
     */
    void setSettingsPageFromOption();

    /**
     * The function computes the remaining time for the meat to reach the desired temperature, based on the current temperature and the rate of change
     */
    void computeRemainingTime();

public:
    /**
     * @brief Construct a new Controller object
     * @param update_interval The interval in milliseconds for the controller to update the view
     */
    Controller(const uint8_t update_interval);
    ~Controller();

    /**
     * @brief The function runs the main controller loop
     */
    void run();

    /**
     * @brief The function sets the page index
     */
    void setPageChange();

    /**
     * @brief The function sets the option change
     * @param direction The direction of the option change
     */
    void setOptionChange(const bool direction);
};

#endif // CONTROLLER_HPP
