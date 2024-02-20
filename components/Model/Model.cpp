#include "Model.hpp"

Model::Model() {
    // Create and add the sensors
    
}

Model::~Model() {
}

void Model::readSensor(uint8_t sensor_id) {
    // find sensor by id and read data
    for(const auto& sensor : sensors_data) {
        if (sensor.sensor_id == sensor_id) {
            sensor.sensor->read();
        }
    }

}

void Model::readAllSensors() {
    for(const auto& sensor : sensors_data) {
        sensor.sensor->read();
    }
}

void Model::addSensorData(uint8_t sensor_id, uint8_t sensor_type, ISensors * sensor) {
    sensors_data[sensor_index].sensor_id = sensor_id;
    sensors_data[sensor_index].sensor_type = sensor_type;
    sensors_data[sensor_index].sensor = sensor;
    sensor_index++;
}

sensor_data Model::readDateFromSensor(uint8_t sensor_id) {
    sensor_data new_data;
    new_data.data_float = 0.0;
    return new_data;
}