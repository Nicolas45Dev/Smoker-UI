#include "Cooker.hpp"

// Private functions for the state machine's states

void Cooker::state_standby()
{
}

void Cooker::state_starting()
{
    GPIO::digitalWrite(MCP23017Pins::EN_MOTOR_PIN, 1); // Enable motor driver

    m_motor.setTargetSpeed(MOTOR_SPEED);

    if (!m_motor.isSoftStarting())
    {
        m_motor.softStart();
    }

    if (m_motor.isRunning())
    {
        m_state = ACTIVE;
        m_previous_tick_motor = m_tick;
    }
}

void Cooker::state_active()
{
    // Run the motor at target speed with the target duty cycle
    if (m_tick - m_previous_tick_motor >= m_duty_cycle_ticks)
    {
        m_previous_tick_motor = m_tick;
    }
    else {
        m_state = PAUSE;
    }
}

void Cooker::state_purging()
{

}

void Cooker::state_control()
{

}

void Cooker::state_filling()
{
    // We assumed the filling comes after a soft start
    // The motor should run for the MOTOR_FILL_TIMEOUT duration
    if (m_tick - m_previous_tick_motor >= MOTOR_FILL_TIMEOUT)
    {
        m_state = STOP;
        m_previous_tick_motor = m_tick;
    }
}

void Cooker::state_waiting()
{
    if (m_tick - m_previous_tick_motor >= (MOTOR_CYCLE - m_duty_cycle_ticks))
    {
        m_state = ACTIVE;
        m_previous_tick_motor = m_tick;
    }
}

void Cooker::state_stopping(bool restart)
{
    if (!m_motor.isSoftStopping())
    {
        m_motor.softStop();
    }

    if (!m_motor.isRunning())
    {
        m_state = STANDBY;
        if (restart)
        {
            m_state = WAITING;
        }
    }
}

void Cooker::temperatureToIntensity()
{
    Monitoring::MilliCelsius temp = m_model->getThermoTankSetTemp();
    COOKER_INTENSITY intensity = COOKER_INTENSITY::COOKER_LOW;

    // Transform the temperature to cooker intensity
    if (temp < 8000) {
        intensity = COOKER_INTENSITY::COOKER_LOW;
    }
    else if (temp >= 8000 && temp < 12000) {
        intensity = COOKER_INTENSITY::COOKER_MIDLOW;
    }
    else if (temp >= 12000 && temp < 16000) {
        intensity = COOKER_INTENSITY::COOKER_MEDIUM;
    }
    else {
        intensity = COOKER_INTENSITY::COOKER_HIGH;
    }

    m_intensity = intensity;
}

// Public functions

Cooker::Cooker()
{
    m_motor.init();
}

Cooker::~Cooker()
{
}

void Cooker::cooker_work()
{
    // Read inputs from the model
    m_thermo_tank = 0;
    m_outside_temp = 0;
    m_is_active = false;

    // State machine work
    switch (m_state)
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
        case FILLING:
            state_filling();
            break;
        case STOP:
            state_stopping();
            break;
        case WAITING:
            break;
        case PAUSE:
            state_stopping(true);
            break;
        default:
            m_state = STANDBY;
            break;
    }

    m_motor.motorRoutine();

    // If the model cooker state is different from the current cooker state, we need to change it
    if (m_model->getCookerState() != m_is_active && m_model->getCookerState())
    {
        m_state = STARTING;
        temperatureToIntensity();
    }

    m_tick++;
}

void Cooker::set_intensity(COOKER_INTENSITY intensity)
{
    m_intensity = intensity;
    m_duty_cycle_ticks = (MOTOR_CYCLE * m_intensity) / 100;
}