#ifndef MONITORING_HPP
#define MONITORING_HPP

#include "hal_adc_esp32.hpp"

const float A = 0.001129148;
const float B = 0.000234125;
const float C = 0.0000000876741;
const float ADC_VOLTAGE_REF = 1.1; // Reference voltage for ADC in volts
const float SERIES_RESISTOR = 10000.0; // Value of the series resistor in ohms
const float NOMINAL_RESISTANCE = 1000.0; // Resistance at 25 degrees C for the thermistor
const float NOMINAL_TEMPERATURE = 25.0; // Nominal temperature value in degrees C

/***
 * @brief Monitoring namespace
 * @details This namespace contains functions to monitor the temperature of the probes and the internal temperature of the ESP32.
 * The monitoring rate is 10Hz, but the raw data is input in a FIR filter to smooth the data. The output of the filter is then converted to Celsius.
 * Each probe is a 1K NTC thermistor in series with a 10k 0.1% resistor. The thermistor is R2 and the resistor is R1 in a voltage divider configuration.
 * The voltage divider is powered by the 3.3V pin of the ESP32 and the output voltage is read by the ADC. The resistance of the thermistor is then calculated using the formula:
 * R2 = R1 * (Vout / (Vin - Vout))
 * The temperature is then calculated using the Steinhart-Hart equation:
 * 1/T = A + B*ln(R) + C*(ln(R))^3
 * Where T is the temperature in Kelvin, R is the resistance of the thermistor, and A, B, and C are the Steinhart-Hart coefficients for the thermistor.
 * @author Nicolas Biard
 * @date 2024-06-10
 */
namespace Monitoring {
    static HalAdcEsp32 adc;
    typedef int32_t Celsius;

    void init();
    Celsius readTemperature_probe1();
    Celsius readTemperature_probe2();
    Celsius readInternalTemperature();
}

#endif // MONITORING_HPP