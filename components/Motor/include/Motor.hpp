#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "Config.h"
#include <driver/ledc.h>
#include <driver/gpio.h>
#include <GPIO.hpp>
#include <esp_log.h>
#include <MCP23017.hpp>
#include <stdint.h>
#include <stdio.h>
#include "sdkconfig.h"

#define MOTOR_PWM_FREQ_HZ 25000
#define MOTOR_DEFAULT_PWM 0
#define MOTOR_MAX_PWM 1023
#define GPIO_MOTOR_PWM (gpio_num_t)CONFIG_MOTOR_PIN_PWM
#define MOTOR_PIN_ENABLE MCP23017Pins::EN_MOTOR_PIN
#define MOTOR_PWM_CHANNEL LEDC_CHANNEL_0
#define MOTOR_PWM_TIMER LEDC_TIMER_0
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
#define MOTOR_PWM_MODE LEDC_HIGH_SPEED_MODE
#else
#define MOTOR_PWM_MODE LEDC_LOW_SPEED_MODE
#endif
#define MOTOR_PWM_DUTY_RES LEDC_TIMER_10_BIT // Set duty resolution to 10 bits
#define MOTOR_CONTROLLER_UPDATE_MS 10
#define DEFAULT_SOFT_STEP 5
#define MOTOR_PERIODIC_UPDATE_MS 10

class Motor {
public:
    Motor();
    ~Motor();

    void init();
    void setSpeed(uint16_t speed);
    void setTargetSpeed(uint16_t speed);
    uint16_t getSpeed() const { return speed; }
    uint16_t getTargetSpeed() const { return target_speed; }
    void softStart();
    void softStop();
    void forceStart();
    void forceStop();

    bool isRunning() const { return motor_running; }
    bool isSoftStarting() const { return soft_starting; }
    bool isSoftStopping() const { return soft_stopping; }

    /**
     * @brief Main motor control routine
     * The routine handles the soft start and soft stop of the motor.
     * It should be called periodically every MOTOR_PERIODIC_UPDATE_MS milliseconds.
     */
    void motorRoutine();

private:
    uint16_t speed = 0;
    uint16_t target_speed = 0;
    int16_t step_speed = 0;
    uint16_t soft_step = DEFAULT_SOFT_STEP;

    bool soft_starting = false;
    bool soft_stopping = false;
    bool motor_running = false;
};

#endif // MOTOR_HPP