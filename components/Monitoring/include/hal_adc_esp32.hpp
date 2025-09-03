#ifndef HAL_ADC_ESP32_HPP
#define HAL_ADC_ESP32_HPP

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "Config.h"
#include <stdint.h>

class HalAdcEsp32 {
public:
    HalAdcEsp32();
    ~HalAdcEsp32();

    void init();
    int readValue(adc_channel_t channel);

private:
    adc_channel_t channel;
};

#endif // HAL_ADC_ESP32_HPP