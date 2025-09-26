#include "Fan.hpp"

Fan::Fan() : m_speed(0), m_isOn(false)
{
}

Fan::~Fan() {
    turnOff();
}

void Fan::init() {
    // Configure the PWM for the fan
    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode = FAN_PWM_MODE;
    ledc_timer.timer_num = FAN_PWM_TIMER;
    ledc_timer.duty_resolution = FAN_PWM_DUTY_RES;
    ledc_timer.freq_hz = FAN_PWM_FREQ_HZ;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel{}; // Ordre: gpio_num, speed_mode, channel, intr_type, timer_sel, duty, hpoint, ...
    ledc_channel.gpio_num = GPIO_FAN_PWM;
    ledc_channel.speed_mode = FAN_PWM_MODE;
    ledc_channel.channel = FAN_PWM_CHANNEL;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.timer_sel = FAN_PWM_TIMER;
    ledc_channel.duty = FAN_DEFAULT_PWM;
    ledc_channel.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    turnOff(); // Ensure the fan is off initially
}

void Fan::setSpeed(int speed) {
    if (speed < 0) speed = 0;
    if (speed > FAN_MAX_PWM) speed = FAN_MAX_PWM;

    m_speed = speed;
    if (m_isOn) {
        if (ledc_set_duty(FAN_PWM_MODE, FAN_PWM_CHANNEL, m_speed) != ESP_OK) {
            ESP_LOGE("FAN", "Failed to set fan speed");
        }
        if (ledc_update_duty(FAN_PWM_MODE, FAN_PWM_CHANNEL) != ESP_OK) {
            ESP_LOGE("FAN", "Failed to update fan speed");
        }
    }
}

void Fan::turnOn() {
    m_isOn = true;

    if (ledc_set_duty(FAN_PWM_MODE, FAN_PWM_CHANNEL, m_speed) != ESP_OK) {
        ESP_LOGE("FAN", "Failed to set fan speed");
    }
    if (ledc_update_duty(FAN_PWM_MODE, FAN_PWM_CHANNEL) != ESP_OK) {
        ESP_LOGE("FAN", "Failed to update fan speed");
    }
}

void Fan::turnOff() {
    m_isOn = false;
    if (ledc_set_duty(FAN_PWM_MODE, FAN_PWM_CHANNEL, 0) != ESP_OK) {
        ESP_LOGE("FAN", "Failed to set fan speed to 0");
    }
    if (ledc_update_duty(FAN_PWM_MODE, FAN_PWM_CHANNEL) != ESP_OK) {
        ESP_LOGE("FAN", "Failed to update fan speed to 0");
    }
}