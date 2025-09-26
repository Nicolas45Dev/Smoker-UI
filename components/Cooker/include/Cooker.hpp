#ifndef COOKER_HPP
#define COOKER_HPP

// Include the configuration file from the Kconfig
#include "sdkconfig.h"
#include "Model.hpp"
#include <driver/gpio.h>
#include "driver/ledc.h"
#include <esp_log.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <Motor.hpp>

#define ENL0_PIN (gpio_num_t)CONFIG_SMOKE_PUMP_PIN
#define ENL1_PIN (gpio_num_t)CONFIG_HEATER_PIN


#define MOTOR_CYCLE 12000 // 2 minutes with 100 Hz tick
#define HEATING_TIMEOUT 30000 // 5 minutes with 100 Hz tick
#define MOTOR_FILL_TIMEOUT 3000
#define MOTOR_SPEED 600
#define INTERVAL_TEMP_CONTROL 100

typedef enum {
    STANDBY,
    STARTING,
    ACTIVE,
    PURGING,
    FILLING,
    WAITING,
    STOP,
    PAUSE,
    CONTROL
} COOKER_STATE;

/**
 * @brief Enum for the cooker intensity levels
 * The levels represent the duty cycle of the motor on a 2 minutes cycle.
 * COOKER_LOW:  1% duty cycle (1.2 seconds on, 118.8 seconds off)
 * COOKER_MIDLOW:  3% duty cycle (3.6 seconds on, 116.4 seconds off)
 * COOKER_MEDIUM:  6% duty cycle (7.2 seconds on, 112.8 seconds off)
 * COOKER_MIDHIGH:  9% duty cycle (10.8 seconds on, 109.2 seconds off)
 * COOKER_HIGH:  12% duty cycle (12 seconds on, 108 seconds off)
 */
typedef enum {
    COOKER_LOW = 1,
    COOKER_MIDLOW = 3,
    COOKER_MEDIUM = 6,
    COOKER_MIDHIGH = 9,
    COOKER_HIGH = 12
} COOKER_INTENSITY;

class Cooker
{
private:
    uint32_t m_tick;

    bool m_is_active = false;
    bool m_is_motor_active = false;

    float m_thermo_tank;
    float m_target_temp;
    float m_outside_temp;

    bool m_motor_open = false;
    uint32_t m_counter = 0;

    // This variable is used to save the last tick at the end of a state.
    uint32_t m_previous_tick_motor = 0;

    uint32_t m_duty_cycle_ticks = 0;

    COOKER_STATE m_state = STANDBY;
    COOKER_INTENSITY m_intensity = COOKER_LOW;
    Motor m_motor;
    Model* m_model = Model::getInstance();

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
     * @brief Define the behavior of the cooker in the stopping state
     * 
     */
    void state_stopping(bool restart = false);

    /**
     * @brief Define the behavior of the cooker in the filling state
     * 
     */
    void state_filling();

    /**
     * @brief Define the behavior of the cooker in the waiting state
     * The state WAITING wait for MOTOR_CYCLE - m_duty_cycle_ticks before going to ACTIVE state
     */
    void state_waiting();

    /**
     * @brief Define the behavior of the cooker in the control state
     * 
     */
    void state_control();

    /**
     * @brief Transform the temperature to intensity
     */
    void temperatureToIntensity();

public:
    Cooker();
    ~Cooker();

    /**
     * @brief Run the state machine of the cooker
     * 
     */
    void cooker_work();

    bool get_active() const { return m_is_active; }
    void set_active(bool active) { m_state = STARTING; }

    COOKER_INTENSITY get_intensity() const { return m_intensity; }
    void set_intensity(COOKER_INTENSITY intensity);
};

#endif