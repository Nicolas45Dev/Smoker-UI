#ifndef HAL_ADC_ESP32_HPP
#define HAL_ADC_ESP32_HPP

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "Config.h"
#include <stdint.h>
#include "singlepolefilter.hpp"

using namespace IIR::SinglePoleFilter;

class HalAdcEsp32 {
public:
    HalAdcEsp32();
    ~HalAdcEsp32();

    void init();
    int readValue(adc_channel_t channel);
    int calibrate(int raw, const char* name);

private:
    adc_channel_t channel;
    adc_oneshot_unit_handle_t handle;

    Filter filter_t1{Filter::computeAlpha(1.0f, 20.0f)}; // Example alpha value for the filter
    Filter filter_t2{Filter::computeAlpha(1.0f, 20.0f)}; // Example alpha value for the filter
    Filter filter_tint{Filter::computeAlpha(1.0f, 20.0f)}; // Example alpha value for the filter
};
    
#endif // HAL_ADC_ESP32_HPP