#ifndef MONITORING_HPP
#define MONITORING_HPP

#include "hal_adc_esp32.hpp"
#include <math.h>
#include "singlepolefilter.hpp"
#include "mobileAverage.hpp"
#include <inttypes.h>

#define MONITORING_TAG "MONITORING"
#define MONITORING_RATE_HZ 25.0f
#define CUTOFF_FREQUENCY_HZ 1.0f

const float BETA = 3.850;

const float ADC_VOLTAGE_REF = 3.3; // Reference voltage for ADC in volts
const float SERIES_RESISTOR = 2200.0; // Value of the series resistor in ohms
const float NOMINAL_RESISTANCE = 1000.0; // Resistance at 25 degrees C for the thermistor
const float NOMINAL_TEMPERATURE = 25.0; // Nominal temperature value in degrees C

/***
 * @brief Monitoring namespace
 * @details This namespace contains functions to monitor the temperature of the probes and the internal temperature of the ESP32.
 * The monitoring rate is 10Hz, but the raw data is input in a FIR filter to smooth the data.
 * @author Nicolas Biard
 * @date 2024-06-10
 */
namespace Monitoring {
    using namespace FIR::MobileAverage;

    static HalAdcEsp32 adc;
    typedef int32_t MilliCelsius;

    static MilliCelsius temperature_t1;
    static MilliCelsius temperature_t2;
    static MilliCelsius temperature_tint;

    static MobileAverage<int, 30> filter_t1; // Moving average filter with window size of 30
    static MobileAverage<int, 30> filter_t2; // Moving average filter with window size of 30
    static MobileAverage<int, 30> filter_tint; // Moving average filter with window size of 30

    void init();
    MilliCelsius readTemperature_probe1();
    MilliCelsius readTemperature_probe2();
    MilliCelsius readInternalTemperature();
}

#endif // MONITORING_HPP