#include <Motor.hpp>

Motor::Motor() : speed(MOTOR_DEFAULT_PWM) {
}

Motor::~Motor() {
}

void Motor::init() {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = MOTOR_PWM_DUTY_RES,
        .freq_hz = MOTOR_PWM_FREQ_HZ,
        .speed_mode = MOTOR_PWM_MODE,
        .timer_num = MOTOR_PWM_TIMER
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = MOTOR_PWM_CHANNEL,
        .duty = MOTOR_DEFAULT_PWM,
        .gpio_num = GPIO_MOTOR_PWM,
        .speed_mode = MOTOR_PWM_MODE,
        .timer_sel = MOTOR_PWM_TIMER
    };
    ledc_channel_config(&ledc_channel);
}

void Motor::setSpeed(uint16_t new_speed) {
    if(new_speed > MOTOR_MAX_PWM) {
        new_speed = MOTOR_MAX_PWM;
    }
    speed = new_speed;
    ledc_set_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL, speed);
    ledc_update_duty(MOTOR_PWM_MODE, MOTOR_PWM_CHANNEL);
}

void Motor::setTargetSpeed(uint16_t new_speed) {
    if(new_speed > MOTOR_MAX_PWM) {
        new_speed = MOTOR_MAX_PWM;
    }
    target_speed = new_speed;
    step_speed = (int16_t)(target_speed - speed) / soft_step;
}

uint16_t Motor::getSpeed() const {
    return speed;
}

uint16_t Motor::getTargetSpeed() const {
    return target_speed;
}

void Motor::softStart() {
    static int8_t counter = 0;

    if(counter++ == soft_step) {
        speed = target_speed;
        counter = 0;
        return;
    }

    this->setSpeed(this->getSpeed() + step_speed);
}

void Motor::softStop() {
    this->setSpeed(this->getSpeed() + step_speed);
}

void Motor::forceStart() {
    setSpeed(MOTOR_MAX_PWM);
}

void Motor::forceStop() {
    setSpeed(0);
}