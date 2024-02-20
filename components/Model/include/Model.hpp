#ifndef MODEL_HPP
#define MODEL_HPP

#include <array>
#include <esp_log.h>
#include <stdio.h>
#include <Sensors.hpp>

typedef union 
{
        float data_float;
        int32_t data_int;
} sensor_data;

typedef struct {
    uint8_t sensor_id;
    uint8_t sensor_type;
    sensor_data data;
    ISensors *sensor;
} sensor_data_t;

/**
 * @brief This class represents the model of the MVC pattern
 * It contains the data from the sensor and the logic to process it
 */
class Model
{
private:
    uint8_t sensor_index = 0;
    std::array<sensor_data_t, 12> sensors_data;

    void addSensorData(uint8_t sensor_id, uint8_t sensor_type, ISensors* sensor);
public:
    Model();
    ~Model();

    void readSensor(uint8_t sensor_id);
    void readAllSensors();

    sensor_data readDateFromSensor(uint8_t sensor_id);
};

#endif // MODEL_HPP