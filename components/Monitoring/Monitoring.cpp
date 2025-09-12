#include <math.h>
#include <stdio.h>
#include "Monitoring.hpp"

namespace Monitoring
{
    void init() {
        // Initialize the ADC for temperature reading
        adc.init();
    }

    MilliCelsius readTemperature_probe1() {
        // Read the temperature from probe 1
        int adc_value = adc.readValue(T1_TEMP);
        adc_value = filter_t1.update(adc_value);
        // Convert ADC value to Celsius using the Steinhart-Hart equation or a lookup table
        // Placeholder conversion, replace with actual calculation
        float voltage = (adc_value * ADC_VOLTAGE_REF) / ADC_MAX_VALUE; // Assuming 12-bit ADC and 3.3V reference
        float resistance = (ADC_VOLTAGE_REF - voltage) * SERIES_RESISTOR / voltage; // Assuming a 10k series resistor
        // Steinhart-Hart coefficients for a typical 1K NTC thermistor
        float lnR = log(resistance);
        float temperatureK = 1.0 / (A + B * lnR + C * lnR * lnR * lnR);
        MilliCelsius temperature = static_cast<MilliCelsius>(temperatureK - 273.15) * 1000; // Convert Kelvin to MilliCelsius

        return temperature;
    }

    MilliCelsius readTemperature_probe2() {
        // Read the temperature from probe 2
        int adc_value = adc.readValue(T2_TEMP); // GPIO35
        adc_value = filter_t2.update(adc_value);
        // Convert ADC value to Celsius using the Steinhart-Hart equation or a lookup table
        // Placeholder conversion, replace with actual calculation
        float voltage = (adc_value * ADC_VOLTAGE_REF) / ADC_MAX_VALUE; // Assuming 12-bit ADC and 3.3V reference
        float resistance = (ADC_VOLTAGE_REF - voltage) * SERIES_RESISTOR / voltage; // Assuming a 10k series resistor
        // Steinhart-Hart coefficients for a typical 1K NTC thermistor
        float lnR = log(resistance);
        float temperatureK = 1.0 / (A + B * lnR + C * lnR * lnR * lnR);
        MilliCelsius temperature = static_cast<MilliCelsius>(temperatureK - 273.15) * 1000; // Convert Kelvin to MilliCelsius

        return temperature;
    }

    MilliCelsius readInternalTemperature() {
        // Read the internal temperature of the ESP32
        // Placeholder implementation, replace with actual reading method
        int adc_value = adc.readValue(TINT_TEMP); // Internal temp sensor channel
        adc_value = filter_tint.update(adc_value);
        // Convert ADC value to Celsius using appropriate formula
        // Placeholder conversion, replace with actual calculation
        MilliCelsius temperature = (adc_value - 500) / 10;
        return temperature;
    }
} // namespace Monitoring
