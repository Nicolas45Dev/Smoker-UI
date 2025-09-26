#ifndef COOKER_HPP
#define COOKER_HPP

// Include the configuration file from the Kconfig
#include "sdkconfig.h"
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


#define MOTOR_TIMEOUT 10000
#define HEATING_TIMEOUT 240000
#define MOTOR_OPEN_TIMEOUT 1000
#define MOTOR_CLOSE_TIMEOUT 600000
#define MOTOR_SPEED 380
#define FILL_TIME 45
#define ON_CYCLE 15
#define OFF_CYCLE 45
#define INTERVAL_TEMP_CONTROL 100

typedef enum {
    STANDBY,
    STARTING,
    ACTIVE,
    PURGING,
    STOP,
    CONTROL
} COOKER_STATE;

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

    uint32_t m_previous_tick_motor = 0;

    COOKER_STATE m_state = STANDBY;
    Motor m_motor;

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
    void state_stopping();

    /**
     * @brief Define the behavior of the cooker in the control state
     * 
     */
    void state_control();

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
};

#endif