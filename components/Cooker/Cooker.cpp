#include "Cooker.hpp"

// Private functions for the state machine's states

void Cooker::state_standby()
{
    // Close motor
    gpio_set_level(ENL1_PIN, 0);
    gpio_set_level(DIRECT_PIN, 0);

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void Cooker::state_starting()
{
    // Turn on the heating element
    gpio_set_level(ENL1_PIN, 1);

    // Turn on the motor for a few seconds
    if(!this->_is_motor_active) {
        gpio_set_level(DIRECT_PIN, 1);

        params_m->channel = LEDC_CHANNEL;
        params_m->speed_mode = LEDC_MODE;
        params_m->speed = 410;

        BaseType_t res = xTaskCreate(start_motor, "start_motor", 2048, (void *)params_m, 6, NULL);
        this->_is_motor_active = true;
        xHandle = xTaskGetHandle("start_motor");
    }

    xHandle = xTaskGetHandle("start_motor");

    if(xHandle == NULL) {
        // acceleration is done
        ESP_LOGI("Cooker", "Linear");
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
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 410);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
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

void Cooker::start_motor(void* params) {
    motor_params* params_motor_task = (motor_params *)params;

    for (uint8_t i = 0; i < params_motor_task->speed / 8; i++) {

        ledc_set_duty(params_motor_task->speed_mode, params_motor_task->channel, i * 8);
        ledc_update_duty(params_motor_task->speed_mode, params_motor_task->channel);

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }

    ledc_set_duty(params_motor_task->speed_mode, params_motor_task->channel, params_motor_task->speed);
    ledc_update_duty(params_motor_task->speed_mode, params_motor_task->channel);

    vTaskDelete(NULL);
}

void Cooker::init_pwm()
{
    ledc_timer_config_t timer_conf;
    memset(&timer_conf, 0, sizeof(ledc_timer_config_t));

    timer_conf.speed_mode = LEDC_MODE;
    timer_conf.duty_resolution = LEDC_DUTY_RES;
    timer_conf.timer_num = LEDC_TIMER;
    timer_conf.freq_hz = LEDC_FREQUENCY;
    timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&timer_conf));

    ledc_channel_config_t ledc_conf;
    memset(&ledc_conf, 0, sizeof(ledc_channel_config_t));

    ledc_conf.channel = LEDC_CHANNEL;
    ledc_conf.duty = 0;
    ledc_conf.gpio_num = LEDC_OUTPUT_IO;
    ledc_conf.speed_mode = LEDC_MODE;
    ledc_conf.timer_sel = LEDC_TIMER;
    ledc_conf.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_conf));

    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);

    ledc_timer_config_t fan_timer_conf;
    memset(&fan_timer_conf, 0, sizeof(ledc_timer_config_t));

    fan_timer_conf.speed_mode = LEDC_MODE;
    fan_timer_conf.duty_resolution = LEDC_FAN_DUTY_RES;
    fan_timer_conf.timer_num = LEDC_FAN_TIMER;
    fan_timer_conf.freq_hz = LEDC_FAN_FREQUENCY;
    fan_timer_conf.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&fan_timer_conf));

    ledc_channel_config_t fan_ledc_conf;
    memset(&fan_ledc_conf, 0, sizeof(ledc_channel_config_t));

    fan_ledc_conf.channel = LEDC_FAN_CHANNEL;
    fan_ledc_conf.duty = 0;
    fan_ledc_conf.gpio_num = LEDC_FAN_OUTPUT_IO;
    fan_ledc_conf.speed_mode = LEDC_MODE;
    fan_ledc_conf.timer_sel = LEDC_FAN_TIMER;
    fan_ledc_conf.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&fan_ledc_conf));

    ledc_set_duty(LEDC_MODE, LEDC_FAN_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_FAN_CHANNEL);
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

    // Open the fan at maximum speed
    ledc_set_duty(LEDC_MODE, LEDC_FAN_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_FAN_CHANNEL);
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