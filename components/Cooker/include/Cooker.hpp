#ifndef COOKER_HPP
#define COOKER_HPP

#include <driver/gpio.h>
#include "driver/ledc.h"
#include <esp_log.h>
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ENL0_PIN (gpio_num_t)35
#define ENL1_PIN (gpio_num_t)39
#define DIRECT_PIN (gpio_num_t)11


#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (12) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_12_BIT // Set duty resolution to 12 bits
#define LEDC_DUTY               (4096) // Set duty to 100%. (2 ** 12) * 100% = 4096
#define LEDC_FREQUENCY          (12000) // Frequency in Hertz. Set frequency at 4 kHz

#define MOTOR_TIMEOUT 10000
#define HEATING_TIMEOUT 240000
#define MOTOR_OPEN_TIMEOUT 1000
#define MOTOR_CLOSE_TIMEOUT 600000

typedef enum {
    STANDBY,
    STARTING,
    ACTIVE,
    PURGING,
    CONTROL
} COOKER_STATE;

class Cooker
{
private:
    uint16_t _interval;

    bool _is_active = false;

    float _thermo_tank;
    float _target_temp;
    float _outside_temp;
    float _humidity;
    float _pressure;

    uint32_t _previous_tick_motor = 0;

    COOKER_STATE _state = STANDBY;

    // Private functions for the state machine's states

    /**
     * @brief Define the behavior of the cooker in the standby state
     * 
     */
    void state_standby();

    /**
     * @brief Define the behavior of the cooker in the starting state
     * 
     */
    void state_starting();

    /**
     * @brief Define the behavior of the cooker in the active state
     * 
     */
    void state_active();

    /**
     * @brief Define the behavior of the cooker in the purging state
     * 
     */
    void state_purging();

    /**
     * @brief Define the behavior of the cooker in the control state
     * 
     */
    void state_control();

    void init_pwm();

public:
    Cooker(const uint16_t interval = 500);
    ~Cooker();

    /**
     * @brief Run the state machine of the cooker
     * 
     */
    void cooker_work();

    void set_active(bool active);
    void set_target_temp(float temp);

};

#endif