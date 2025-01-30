#ifndef MODEL_HPP
#define MODEL_HPP

#include <array>
#include "Config.h"
#include <esp_log.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <MAX31855.hpp>
#include <BME280.hpp>
#include "sdkconfig.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef CONFIG_SENSOR_TYPE_THERMOCOUPLE
#define THERMO_TANK_CS (gpio_num_t)CONFIG_THERMOCOUPLE_TANK_SPI_CS
#define THERMO_MEAT1_CS (gpio_num_t)CONFIG_THERMOCOUPLE_MEAT1_SPI_CS
#define THERMO_MEAT2_CS (gpio_num_t)CONFIG_THERMOCOUPLE_MEAT2_SPI_CS

#define THERMO_TANK_ACTIVE_LED (gpio_num_t)8
#define THERMO_MEAT1_ACTIVE_LED (gpio_num_t)10
#define THERMO_MEAT2_ACTIVE_LED (gpio_num_t)9
#endif

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

    BME280 bme280;
    MAX31855 thermo_tank;
    MAX31855 thermo_meat1;
    MAX31855 thermo_meat2;

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

    void readThermocouples(char* data, uint8_t sensor_index);
    void readBME280(uint8_t sensor_index, char* data);
    float getThermoTankSetTemp() { return thermo_tank_set_temp; }

    void getThermoMeat1SetTemp(char* data);
    void getThermoMeat2SetTemp(char* data);
    uint8_t getPageIndex() { return page_index; }
    uint8_t getPageOption() { return page_option; }
    uint32_t getThermoRemindTime(uint8_t sensor_index);

    float readThermocouples(uint8_t sensor_index);
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