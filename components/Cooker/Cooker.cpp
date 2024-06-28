#include "Cooker.hpp"

// Private functions for the state machine's states

void Cooker::state_standby()
{

}

void Cooker::state_starting()
{

}

void Cooker::state_active()
{

}

void Cooker::state_purging()
{

}

void Cooker::state_control()
{

}

// Public functions

Cooker::Cooker(const uint16_t interval)
{
    _interval = interval;
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
            break;
    }

    // Update the ouputs
}