#ifndef COOKER_HPP
#define COOKER_HPP

// Include the configuration file from the Kconfig
#include "sdkconfig.h"
#include "Model.hpp"
#include <driver/gpio.h>
#include "driver/ledc.h"
#include <esp_log.h>
#include <GPIO.hpp>
#include <Fan.hpp>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <Motor.hpp>
#include <Model.hpp>

#define MOTOR_TIMEOUT 10000
#define HEATING_TIMEOUT 24000
#define MOTOR_OPEN_TIMEOUT 1000
#define MOTOR_CLOSE_TIMEOUT 60000
#define MOTOR_FILL_TIMEOUT 30000
#define MOTOR_SPEED 380
#define MOTOR_CYCLE 12000 // 2 minutes cycle
#define INTERVAL_TEMP_CONTROL 100
#define FAN_TIMEOUT 24000

typedef enum {
    STANDBY,
    STARTING,
    HEATING,
    ACTIVE,
    PURGING,
    FILLING,
    WAITING,
    PAUSE,
    STOP,
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
    uint32_t m_previous_tick_fan = 0;
    uint32_t m_duty_cycle_ticks = 0;

    COOKER_STATE m_state = STANDBY;
    COOKER_INTENSITY m_intensity = COOKER_LOW;
    Motor m_motor;
    Fan m_fan;
    Model* m_model;

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
    void state_starting(bool heating = false);

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
     * @brief Define the behavior of the cooker in the heating state
     * 
     */
    void state_heating();

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
     * @brief Transform the temperature to intensity
     */
    void temperatureToIntensity();

    /**
     * @brief Define the behavior of the cooker in the control state
     * 
     */
    void state_control(uint32_t target_speed);

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

    void init() 
    {
        m_model = Model::getInstance();
        m_duty_cycle_ticks = (MOTOR_CYCLE * m_intensity) / 100;
    }
};

extern Cooker g_instance_cooker;

#endif