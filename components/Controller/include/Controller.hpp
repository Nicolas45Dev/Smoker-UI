#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <esp_log.h>
#include <stdio.h>
#include <View.hpp>
#include <stdint.h>
#include <BME280.hpp>

#define THERMOMETER_UPDATE_INTERVAL 20
#define THERMOCOUPLE_UPDATE_INTERVAL 20

class Controller
{
private:
    uint8_t bme280_update_tick = 0;
    uint8_t thermocouple_update_tick = 0;
    uint32_t time_tick = 0;
    View view;

    BME280 bme280;

    void readModelData();
    void updateView();
    void updateModel();
public:
    Controller(uint8_t update_interval);
    ~Controller();

    void run();
};

#endif // CONTROLLER_HPP
