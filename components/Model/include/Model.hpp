#ifndef MODEL_HPP
#define MODEL_HPP

#include <array>
#include "Config.h"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <BME280.hpp>
#include "Monitoring.hpp"
#include "mobileAverage.hpp"
#include "sdkconfig.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>


static const char* TAG = "MODEL";
const int BME280_UPDATE_PERIOD_TICK = 50 * 2; // Update BME280 every 2 seconds with 50 ms tick
const Monitoring::MilliCelsius DEFAULT_SET_TEMP = 2500; // Default set temperature in MilliCelsius (22.5°C)

/**
 * @brief This class represents the model of the MVC pattern
 * It contains the data from the sensor and the logic to process it
 */
class Model
{
private:
    uint8_t page_index = 0;
    uint8_t page_option = 0;

    uint32_t bme280_tick = 0;

    bool page_change = false;

    Monitoring::MilliCelsius temperature_t1 = 0;
    Monitoring::MilliCelsius temperature_t2 = 0;
    Monitoring::MilliCelsius temperature_tint = 0;

    TEMP_UNIT user_unit = DEFAULT_UNIT;

    BME280_Device::BME280 bme280 = BME280_Device::BME280();

    FIR::MobileAverage::MobileAverage<Monitoring::MilliCelsius, 5> temperature_average;

    static Model* _instance;

protected:
    Model();
    ~Model();

public:
    Model(Model const&) = delete;
    void operator=(Model const&) = delete;

    static Model* getInstance();

    void readBME280();
    float getBME280Temperature();
    float getBME280Pressure();
    float getThermoTankSetTemp() { return 0.0f; }

    void getThermoMeat1SetTemp(char* data);
    void getThermoMeat2SetTemp(char* data);
    uint8_t getPageIndex() { return page_index; }
    uint8_t getPageOption() { return page_option; }
    uint32_t getThermoRemindTime(uint8_t sensor_index);

    Monitoring::MilliCelsius readThermometers(uint8_t sensor_index);
    void reset();
    void setThermoTankSetTemp(float temp) { return; }
    void setThermoMeat1SetTime(char* time);
    void setThermoMeat2SetTime(char* time);
    void setThermoMeat1SetTemp(float temp) { return; }
    void setThermoMeat2SetTemp(float temp) { return; }

    /**
     * @brief Reads all the sensors and updates the data
     * ! This function should be called periodically
     * @note The function has a slower tick to update the BME280 sensor
     */
    void update();
};

#endif // MODEL_HPP