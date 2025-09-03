#include <math.h>
#include <stdio.h>
#include "Monitoring.hpp"

namespace Monitoring
{
    void init() {
        // Initialize the ADC for temperature reading
        adc.init();
    }

    Celsius readTemperature_probe1() {
        // Read the temperature from probe 1
        int adc_value = adc.readValue(ADC_CHANNEL_6); // GPIO34
        // Convert ADC value to Celsius using the Steinhart-Hart equation or a lookup table
        // Placeholder conversion, replace with actual calculation
        float voltage = (adc_value / 4095.0) * 3.3; // Assuming 12-bit ADC and 3.3V reference
        float resistance = (3.3 - voltage) * 10000 / voltage; // Assuming a 10k series resistor
        // Steinhart-Hart coefficients for a typical 1K NTC thermistor
        float lnR = log(resistance);
        float temperatureK = 1.0 / (A + B * lnR + C * lnR * lnR * lnR);
        Celsius temperature = static_cast<Celsius>(temperatureK - 273.15); // Convert Kelvin to Celsius

        return temperature;
    }

    Celsius readTemperature_probe2() {
        // Read the temperature from probe 2
        int adc_value = adc.readValue(ADC_CHANNEL_7); // GPIO35
        // Convert ADC value to Celsius using the Steinhart-Hart equation or a lookup table
        // Placeholder conversion, replace with actual calculation
        float voltage = (adc_value / 4095.0) * 3.3; // Assuming 12-bit ADC and 3.3V reference
        float resistance = (3.3 - voltage) * 10000 / voltage; // Assuming a 10k series resistor
        // Steinhart-Hart coefficients for a typical 1K NTC thermistor
        float lnR = log(resistance);
        float temperatureK = 1.0 / (A + B * lnR + C * lnR * lnR * lnR);
        Celsius temperature = static_cast<Celsius>(temperatureK - 273.15); // Convert Kelvin to Celsius

        return temperature;
    }

    Celsius readInternalTemperature() {
        // Read the internal temperature of the ESP32
        // Placeholder implementation, replace with actual reading method
        int adc_value = adc.readValue(ADC_CHANNEL_0); // Internal temp sensor channel
        // Convert ADC value to Celsius using appropriate formula
        // Placeholder conversion, replace with actual calculation
        Celsius temperature = (adc_value - 500) / 10;
        return temperature;
    }
} // namespace Monitoring
