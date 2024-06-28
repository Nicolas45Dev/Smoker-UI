#ifndef COOKER_HPP
#define COOKER_HPP

#include <driver/gpio.h>
#include <esp_log.h>
#include <Model.hpp>
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

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
    float _outside_temp;
    float _humidity;
    float _pressure;

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

public:
    Cooker(const uint16_t interval = 500);
    ~Cooker();

    /**
     * @brief Run the state machine of the cooker
     * 
     */
    void cooker_work();
};

#endif