#ifndef MAX31855_HPP
#define MAX31855_HPP

#include <driver/gpio.h>
#include <SPI.hpp>
#include <stdint.h>
#include <string.h>
#include <esp_log.h>

typedef enum
{
    CELSIUS_UNIT,
    FAHRENHEIT_UNIT,
    KELVIN_UNIT
} TEMP_UNIT;

// define a macro for temperature conversion
#define CELSIUS_TO_FAHRENHEIT(x) (x * 9 / 5 + 32)
#define CELSIUS_TO_KELVIN(x) (x + 273.15)
#define FAHRENHEIT_TO_CELSIUS(x) ((x - 32) * 5 / 9)
#define FAHRENHEIT_TO_KELVIN(x) ((x - 32) * 5 / 9 + 273.15)
#define KELVIN_TO_CELSIUS(x) (x - 273.15)
#define KELVIN_TO_FAHRENHEIT(x) ((x - 273.15) * 9 / 5 + 32)

#define OPEN_CIRCUIT_FAULT 0x01

class MAX31855
{
private:
    uint8_t avg_index = 0;

    float temperature_probe = 0;
    float internal_temperature = 0;
    SPI* spi;

    float avg_temperature = 0;
    float temperature[5];

    gpio_num_t pin_cs;
    gpio_num_t active_led;

public:
    MAX31855(gpio_num_t pin_cs, gpio_num_t active_led);
    ~MAX31855();

    /**
     * @brief Check if the probe is connected
     * NOTE: The function should be called before reading the temperature
     * @return true The probe is connected
     * @return false The probe is not connected
     */
    bool isProbeConnected();

    /**
     * @brief Read the temperature from the MAX31855
     * NOTE: The temperature is averaged over 5 readings
     * NOTE: The function isProbeConnected() should be called before this function
     * @param unit The unit to read the temperature in
     * @return float The temperature in the specified unit
     */
    float readTemperature(TEMP_UNIT unit = TEMP_UNIT::CELSIUS_UNIT);
};

#endif // MAX31855_HPP
