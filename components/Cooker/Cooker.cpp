#include "Cooker.hpp"

// Private functions for the state machine's states

void Cooker::state_standby()
{
}

void Cooker::state_starting()
{
    GPIO::digitalWrite(MCP23017Pins::EN_MOTOR_PIN, 1); // Enable motor driver

    m_motor.setTargetSpeed(750);

    if (!m_motor.isSoftStarting())
    {
        m_motor.softStart();
    }

    if (m_motor.isRunning())
    {
        m_state = ACTIVE;
    }
}

void Cooker::state_active()
{
    // Temperature control logic every INTERVAL_TEMP_CONTROL ticks
    if (m_tick % INTERVAL_TEMP_CONTROL == 0)
    {
        // Temperature control logic here
    }
}

void Cooker::state_purging()
{

}

void Cooker::state_control()
{

}

void Cooker::state_stopping()
{
    if (!m_motor.isSoftStopping())
    {
        m_motor.softStop();
    }

    if (!m_motor.isRunning())
    {
        m_state = STANDBY;
    }
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
        default:
            m_state = STANDBY;
            break;
    }

    m_motor.motorRoutine();

    m_tick++;
}