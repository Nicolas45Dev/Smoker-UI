#include <Motor.hpp>

Motor::Motor() : speed(MOTOR_DEFAULT_PWM) {
}

Motor::~Motor() {
}

void Motor::init() {
    // Initialisation compatible C++ (évite l’usage des initialisateurs désignés qui exigent l’ordre exact sous C++20)
    ledc_timer_config_t ledc_timer{}; // zero-init
    // Ordre déclaré dans ledc_timer_config_t (ESP-IDF 5.x) : speed_mode, timer_num, duty_resolution, freq_hz, clk_cfg, ...
    ledc_timer.speed_mode = MOTOR_PWM_MODE;
    ledc_timer.timer_num = MOTOR_PWM_TIMER;
    ledc_timer.duty_resolution = MOTOR_PWM_DUTY_RES;
    ledc_timer.freq_hz = MOTOR_PWM_FREQ_HZ;
    ledc_timer.clk_cfg = LEDC_AUTO_CLK;
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    ledc_channel_config_t ledc_channel{}; // Ordre: gpio_num, speed_mode, channel, intr_type, timer_sel, duty, hpoint, ...
    ledc_channel.gpio_num = GPIO_MOTOR_PWM;
    ledc_channel.speed_mode = MOTOR_PWM_MODE;
    ledc_channel.channel = MOTOR_PWM_CHANNEL;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.timer_sel = MOTOR_PWM_TIMER;
    ledc_channel.duty = MOTOR_DEFAULT_PWM;
    ledc_channel.hpoint = 0;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void Motor::setSpeed(uint16_t new_speed) {
    speed = new_speed;
}

void Motor::setTargetSpeed(uint16_t new_speed) {
    target_speed = new_speed;
}

void Motor::softStart() {
    soft_starting = true;
}

void Motor::softStop() {
    soft_stopping = true;
}

void Motor::forceStart() {
    setSpeed(MOTOR_MAX_PWM);
}

void Motor::forceStop() {
    motor_running = false;
}

void Motor::motorRoutine() {
    if (motor_running) {
        // Update PWM to target speed
        ledc_set_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL, speed);
        ledc_update_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL);
    }

    // Handle soft starting
    if (soft_starting) {
        // GPIO::digitalWrite(MCP23017Pins::EN_MOTOR_PIN, 1); // Enable motor driver
        if (speed < target_speed) 
        {
            speed += soft_step;
            if (speed >= target_speed) 
            {
                speed = target_speed;
                soft_starting = false;
            }
            ledc_set_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL, speed);
            ledc_update_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL);
        } 
        else 
        {
            soft_starting = false;
            motor_running = true; // Motor is now running
            speed = target_speed; // Ensure speed is set to target
        }
    }

    // Handle soft stopping
    if (soft_stopping) {
        if (speed > 0) 
        {
            speed -= soft_step;
            if (speed <= 0)
            {
                speed = 0;
            }
            ledc_set_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL, speed);
            ledc_update_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL);
        }
        else 
        {
            printf("Motor stopped\n");
            soft_stopping = false;
            motor_running = false; // Motor is now stopped
            speed = 0; // Ensure speed is zero
            GPIO::digitalWrite(MCP23017Pins::EN_MOTOR_PIN, 0); // Disable motor driver
        }
    }
}