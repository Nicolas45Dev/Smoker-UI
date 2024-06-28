#ifndef MODEL_HPP
#define MODEL_HPP

#include <array>
#include "Config.h"
#include <esp_log.h>
#include <MAX31855.hpp>
#include "MeatProfileData.h"
#include <BME280.hpp>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define THERMO_TANK_CS (gpio_num_t)19
#define THERMO_MEAT1_CS (gpio_num_t)20
#define THERMO_MEAT2_CS (gpio_num_t)21

#define THERMO_TANK_ACTIVE_LED (gpio_num_t)8
#define THERMO_MEAT1_ACTIVE_LED (gpio_num_t)10
#define THERMO_MEAT2_ACTIVE_LED (gpio_num_t)9

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

    float thermo_tank_set_temp = 0;
    float thermo_meat1_set_temp = 0;
    float thermo_meat2_set_temp = 0;

    BME280 bme280;
    MAX31855 thermo_tank;
    MAX31855 thermo_meat1;
    MAX31855 thermo_meat2;

    TEMP_UNIT user_unit = DEFAULT_UNIT;

    std::array<MeatProfileData, 10> meat_profile_data;

public:
    Model();
    ~Model();

    uint8_t getPageIndex() const { return page_index; }
    uint8_t getPageOption() const { return page_option; }

    void getThermoMeat1SetTemp(char* data) const;
    void getThermoMeat2SetTemp(char* data) const;
    void readBME280(const uint8_t sensor_index, char* data);
    void readThermocouples(char* data, const uint8_t sensor_index);
    void setPageChange(bool change, int8_t option_change = 0);
    void setPageIndex(const uint8_t index) { page_index = index; }
    void setThermoMeat1SetTemp(const float temp) { thermo_meat1_set_temp = temp; }
    void setThermoMeat2SetTemp(const float temp) { thermo_meat2_set_temp = temp; }
    void setThermoTankSetTemp(const float temp) { thermo_tank_set_temp = temp; }
    void setUnit(const TEMP_UNIT unit) { user_unit = unit; }

    float getThermoMeat1SetTemp() const { return thermo_meat1_set_temp; }
    float getThermoMeat2SetTemp() const { return thermo_meat2_set_temp; }
    float getThermoTankSetTemp() const { return thermo_tank_set_temp; }
    float readThermocouples(uint8_t sensor_index);

    std::array<MeatProfileData, 10> &getMeatProfile() { return meat_profile_data; }

};

#endif // MODEL_HPP