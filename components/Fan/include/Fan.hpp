#ifndef FAN_HPP
#define FAN_HPP

#include "Config.h"
#include <driver/ledc.h>
#include <driver/gpio.h>
#include <GPIO.hpp>
#include <esp_log.h>
#include <MCP23017.hpp>
#include <stdint.h>
#include <stdio.h>
#include "sdkconfig.h"

// Configurations for a ledc channel with 10 bits resolution and 1000 Hz frequency
#define FAN_PWM_FREQ_HZ 1000
#define FAN_DEFAULT_PWM 0
#define FAN_MAX_PWM 1023
#define GPIO_FAN_PWM (gpio_num_t)CONFIG_FAN_PIN_PWM
#define FAN_PWM_CHANNEL LEDC_CHANNEL_1
#define FAN_PWM_TIMER LEDC_TIMER_0
#ifdef __has_include
#  if __has_include(<esp_idf_version.h>)
#    include <esp_idf_version.h>
#  endif
#endif
#ifndef ESP_IDF_VERSION_VAL
// Fallback definition if header not found (assume IDF >=5 so only low speed mode exists)
#define ESP_IDF_VERSION_VAL(major, minor, patch) ((major)*10000 + (minor)*100 + (patch))
#define ESP_IDF_VERSION ESP_IDF_VERSION_VAL(5,0,0)
#endif
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
#define FAN_PWM_MODE LEDC_HIGH_SPEED_MODE
#else
#define FAN_PWM_MODE LEDC_LOW_SPEED_MODE
#endif
#define FAN_PWM_DUTY_RES LEDC_TIMER_10_BIT // Set duty resolution to 10 bits

class Fan {
public:
    Fan();
    ~Fan();

    void init();
    void setSpeed(int speed);
    void turnOn();
    void turnOff();

private:
    int m_speed;
    bool m_isOn;
};

#endif // FAN_HPP