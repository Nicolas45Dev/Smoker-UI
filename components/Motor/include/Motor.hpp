#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <driver/ledc.h>
#include <driver/gpio.h>
#include <GPIO.hpp>
#include <esp_log.h>
#include <stdint.h>
#include <stdio.h>

#define MOTOR_PWM_FREQ_HZ 25000
#define MOTOR_DEFAULT_PWM 0
#define MOTOR_PWM_CHANNEL LEDC_CHANNEL_0
#define MOTOR_PWM_TIMER LEDC_TIMER_0
#define MOTOR_PWM_MODE LEDC_HIGH_SPEED_MODE
#define MOTOR_PWM_DUTY_RES LEDC_TIMER_10_BIT // Set duty resolution to 10 bits
#define MOTOR_CONTROLLER_UPDATE_MS 10
#define DEFAULT_SOFT_STEP 50

class Motor {
public:
    Motor();
    ~Motor();

    void init();
    void setSpeed(uint16_t speed);
    void setTargetSpeed(uint16_t speed);
    uint16_t getSpeed() const;
    uint16_t getTargetSpeed() const { return target_speed; }
    void softStart();
    void softStop();
    void forceStart();
    void forceStop();

private:
    uint16_t speed;
    uint16_t target_speed;
    int16_t step_speed;
    uint16_t soft_step = DEFAULT_SOFT_STEP;
};

#endif // MOTOR_HPP