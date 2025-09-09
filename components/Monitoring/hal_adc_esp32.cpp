#include "hal_adc_esp32.hpp"

HalAdcEsp32::HalAdcEsp32() {
    // Constructor implementation
}

HalAdcEsp32::~HalAdcEsp32() {
    // Destructor implementation
}

void HalAdcEsp32::init() {
    // Initialize the ADC
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    adc_oneshot_new_unit(&init_config, &handle);

    adc_oneshot_chan_cfg_t chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, T1_TEMP, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, T2_TEMP, &chan_config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(handle, TINT_TEMP, &chan_config));
}

int HalAdcEsp32::readValue(adc_channel_t channel) {

    static int adc_reading = 0;

    switch (channel)
    {
        case T1_TEMP:
            // Read the ADC value from channel 0
            ESP_ERROR_CHECK(adc_oneshot_read(handle, T1_TEMP, &adc_reading));
            adc_reading = calibrate(adc_reading, "T1_TEMP");
            adc_reading = filter_t1.update(adc_reading);
            break;
        case T2_TEMP:
            // Read the ADC value from channel 1
            ESP_ERROR_CHECK(adc_oneshot_read(handle, T2_TEMP, &adc_reading));
            adc_reading = calibrate(adc_reading, "T2_TEMP");
            adc_reading = filter_t2.update(adc_reading);
            break;
        case TINT_TEMP:
            // Read the ADC value from channel 2
            ESP_ERROR_CHECK(adc_oneshot_read(handle, TINT_TEMP, &adc_reading));
            adc_reading = calibrate(adc_reading, "TINT_TEMP");
            adc_reading = filter_tint.update(adc_reading);
            break;
        default:
            break;
    }
    return adc_reading;
}

int HalAdcEsp32::calibrate(int raw, const char* name) {
    // Simple linear calibration example
    // Adjust the slope and intercept based on your calibration data
    float slope = 1.0;
    float intercept = 0.0;
    // read from the Flash the calibration parameters for the given name
    return static_cast<int>(slope * (float)raw + intercept);
}