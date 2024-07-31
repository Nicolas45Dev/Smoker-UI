#include "Cooker.hpp"

// Private functions for the state machine's states

void Cooker::state_standby()
{

}

void Cooker::state_starting()
{
    // Turn on the heating element
    gpio_set_level(ENL1_PIN, 1);



    // Turn on the motor for a few seconds
    if(xTaskGetTickCount() - _previous_tick_motor < MOTOR_TIMEOUT) {
        gpio_set_level(DIRECT_PIN, 1);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 1800);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    }
    else {
        gpio_set_level(DIRECT_PIN, 0);
    }

    if(xTaskGetTickCount() - _previous_tick_motor > HEATING_TIMEOUT) {
        gpio_set_level(ENL1_PIN, 0);
        _state = ACTIVE;
        _previous_tick_motor = xTaskGetTickCount();
    }
}

void Cooker::state_active()
{
    // Open the motor for 1 second every 5 minutes
    if(xTaskGetTickCount() - _previous_tick_motor > MOTOR_OPEN_TIMEOUT) {
        gpio_set_level(DIRECT_PIN, 1);
        _previous_tick_motor = xTaskGetTickCount();
    }
    else if(xTaskGetTickCount() - _previous_tick_motor > MOTOR_CLOSE_TIMEOUT) {
        gpio_set_level(DIRECT_PIN, 0);
    }
}

void Cooker::state_purging()
{

}

void Cooker::state_control()
{

}

void Cooker::init_pwm()
{
    ledc_timer_config_t timer_conf;
    timer_conf.speed_mode = LEDC_MODE;
    timer_conf.duty_resolution = LEDC_DUTY_RES;
    timer_conf.timer_num = LEDC_TIMER;
    timer_conf.freq_hz = LEDC_FREQUENCY;
    timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timer_conf);

    ledc_channel_config_t ledc_conf;
    ledc_conf.channel = LEDC_CHANNEL;
    ledc_conf.duty = 0;
    ledc_conf.gpio_num = LEDC_OUTPUT_IO;
    ledc_conf.speed_mode = LEDC_MODE;
    ledc_conf.timer_sel = LEDC_TIMER;
    ledc_conf.hpoint = 0;
    ledc_channel_config(&ledc_conf);
}

// Public functions

Cooker::Cooker(const uint16_t interval)
{
    _interval = interval;

    // Initialize the GPIOs
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << ENL0_PIN) | (1ULL << ENL1_PIN) | (1ULL << DIRECT_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // Initialize the PWM
    init_pwm();

    gpio_set_level(ENL0_PIN, 0);
    gpio_set_level(ENL1_PIN, 0);
    gpio_set_level(DIRECT_PIN, 0);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

Cooker::~Cooker()
{
}

void Cooker::cooker_work()
{
    // Read inputs from the model
    _thermo_tank = 0;
    _outside_temp = 0;
    _humidity = 0;
    _pressure = 0;
    _is_active = false;

    // State machine work
    switch (_state)
    {
        case STANDBY:
            state_standby();
            break;
        case STARTING:
            state_starting();
            break;
        case ACTIVE:
            state_active();
            break;
        case PURGING:
            state_purging();
            break;
        case CONTROL:
            state_control();
            break;
        default:
            _state = STANDBY;
            _previous_tick_motor = xTaskGetTickCount();
            break;
    }

    // Update the ouputs
}

void Cooker::set_active(bool active)
{
    this->_is_active = active;
    if(active) {
        this->_state = STARTING;
    } else {
        this->_state = STANDBY;
    }
}

void Cooker::set_target_temp(float temp)
{
    // Set the target temperature
    this->_target_temp = temp;
}