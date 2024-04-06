#ifndef MODEL_HPP
#define MODEL_HPP

#include <array>
#include <esp_log.h>
#include <MAX31855.hpp>
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
    BME280 bme280;
    MAX31855 thermo_tank;
    MAX31855 thermo_meat1;
    MAX31855 thermo_meat2;

    float thermo_tank_set_temp = 0;
    float thermo_meat1_set_temp = 0;
    float thermo_meat2_set_temp = 0;

public:
    Model();
    ~Model();

    void readThermocouples(char* data, uint8_t sensor_index);
    float readBME280(uint8_t sensor_index);
    float getThermoTankSetTemp() { return thermo_tank_set_temp; }
    float getThermoMeat1SetTemp() { return thermo_meat1_set_temp; }
    float getThermoMeat2SetTemp() { return thermo_meat2_set_temp; }
};

#endif // MODEL_HPP