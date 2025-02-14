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

#define ENL0_PIN (gpio_num_t)CONFIG_SMOKE_PUMP_PIN
#define ENL1_PIN (gpio_num_t)CONFIG_HEATER_PIN
#define DIRECT_PIN (gpio_num_t)CONFIG_MOTOR_PIN_DIRECTION


#define LEDC_TIMER              LEDC_TIMER_1
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (CONFIG_MOTOR_PIN_PWM) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_1
#define LEDC_DUTY_RES           LEDC_TIMER_10_BIT // Set duty resolution to 12 bits
#define LEDC_DUTY               (0) // Set duty to 100%. (2 ** 10) * 100% = 4096
#define LEDC_FREQUENCY          (25000) // Frequency in Hertz. Set frequency at 25 kHz

#define LEDC_FAN_TIMER          LEDC_TIMER_2   // Nouveau timer pour le FAN
#define LEDC_FAN_CHANNEL        LEDC_CHANNEL_2 // Nouveau canal pour le FAN
#define LEDC_FAN_OUTPUT_IO      (CONFIG_FAN_PIN_PWM) // Define the output GPIO
#define LEDC_FAN_FREQUENCY      (1000)          // 1000 Hz
#define LEDC_FAN_DUTY_RES       LEDC_TIMER_10_BIT // Résolution 10 bits
#define LEDC_FAN_DUTY           (720)


#define MOTOR_TIMEOUT 10000
#define HEATING_TIMEOUT 240000
#define MOTOR_OPEN_TIMEOUT 1000
#define MOTOR_CLOSE_TIMEOUT 600000
#define MOTOR_SPEED 380
#define FILL_TIME 45
#define ON_CYCLE 15

typedef enum {
    STANDBY,
    STARTING,
    ACTIVE,
    PURGING,
    CONTROL
} COOKER_STATE;

typedef struct {
    uint16_t speed;
    ledc_mode_t speed_mode;
    ledc_channel_t channel;
} motor_params;

class Cooker
{
private:
    uint16_t _interval;

    bool _is_active = false;
    bool _is_motor_active = false;

    float _thermo_tank;
    float _target_temp;
    float _outside_temp;
    float _humidity;
    float _pressure;

    static bool motor_open = false;
    static uint32_t counter = 0;

    motor_params* params_m = (motor_params *)malloc(sizeof(motor_params));

    uint32_t _previous_tick_motor = 0;

    COOKER_STATE _state = STANDBY;

    TaskHandle_t xHandle;
    TimerHandle_t xTimer;

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

    bool get_active() const { return _is_active; }
    void set_active(bool active);
    void set_travel(float travel);
    void set_target_temp(float temp);

    static void start_motor(void* params);

    static void timer_callback(TimerHandle_t xTimer) {
        ESP_LOGI("TIMER", "Timer expired");
    }

};

#endif