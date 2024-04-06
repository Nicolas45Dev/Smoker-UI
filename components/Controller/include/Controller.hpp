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

#define THERMOMETER_UPDATE_INTERVAL 20
#define THERMOCOUPLE_UPDATE_INTERVAL 20

class Controller
{
private:
    uint8_t bme280_update_tick = 0;
    uint8_t thermocouple_update_tick = 0;
    uint32_t time_tick = 0;

    uint8_t sensor_size = 0;

    char thermo_tank[8];
    char thermo_meat1[8];
    char thermo_meat2[8];

    float bme280_data[3];

    View view;
    Model model;

    void readModelData();
    void updateView();
    void updateModel();
public:
    Controller(uint8_t update_interval);
    ~Controller();

    void run();
};

#endif // CONTROLLER_HPP
