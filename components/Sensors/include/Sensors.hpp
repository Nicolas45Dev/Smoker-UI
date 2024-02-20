#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <esp_log.h>
#include <stdio.h>

/**
 * @brief This class represents the interface for the sensors
 * 
 */
class ISensors
{
public:
    ISensors() = default;
    ~ISensors() = default;
    virtual void read() = 0;
    virtual bool init() = 0;
};

#endif // SENSORS_HPP