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
#include "sdkconfig.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief This class represents the model of the MVC pattern
 * It contains the data from the sensor and the logic to process it
 */
class Model
{
private:
    uint8_t page_index = 0;
    uint8_t page_option = 0;

    bool page_change = false;

    float thermo_tank_set_temp = 0.0f;
    float thermo_meat1_set_temp = 0.0f;
    float thermo_meat2_set_temp = 0.0f;
    float previous_thermo_temp_1 = 0.0f;
    float previous_thermo_temp_2 = 0.0f;
    float thermo_remind_time_1, thermo_remind_time_2;

    TEMP_UNIT user_unit = DEFAULT_UNIT;

    SemaphoreHandle_t _mutex = NULL;
    static Model* _instance;

protected:
    Model();
    ~Model();

public:
    Model(Model const&) = delete;
    void operator=(Model const&) = delete;

    static Model* getInstance();

    void readThermometers(char* data, uint8_t sensor_index);
    void readBME280();
    float getThermoTankSetTemp() { return thermo_tank_set_temp; }

    void getThermoMeat1SetTemp(char* data);
    void getThermoMeat2SetTemp(char* data);
    uint8_t getPageIndex() { return page_index; }
    uint8_t getPageOption() { return page_option; }
    uint32_t getThermoRemindTime(uint8_t sensor_index);

    float readThermometers(uint8_t sensor_index);
    void reset();
    void setThermoTankSetTemp(float temp) { thermo_tank_set_temp = temp; }
    void setThermoMeat1SetTime(char* time);
    void setThermoMeat2SetTime(char* time);
    void setThermoMeat1SetTemp(float temp) { thermo_meat1_set_temp = temp; }
    void setThermoMeat2SetTemp(float temp) { thermo_meat2_set_temp = temp; }
    void setPageChange(bool change, int8_t option_change = 0);
    void setPageIndex(uint8_t index) { page_index = index; }
};

#endif // MODEL_HPP