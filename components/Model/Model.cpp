#include "Model.hpp"

Model* Model::_instance = NULL;

Model::Model()
{
    Monitoring::init();
    bme280.init();
}

Model::~Model() {
}

Model* Model::getInstance() {
    if(_instance == NULL) {
        _instance = new Model();
    }
    return _instance;
}

Monitoring::MilliCelsius Model::readThermometers(uint8_t sensor_index) {
    Monitoring::MilliCelsius temp = 0;
    switch (sensor_index) {
        case 0:
            temp = temperature_t1;
            break;
        case 1:
            temp = temperature_t2;
            break;
        case 2:
            temp = temperature_tint;
            break;
        default:
            temp = DEFAULT_SET_TEMP; // Error value
            break;
    }
    return temp;
}

void Model::readBME280() {
    BME280_Device::bme280.readAll();
}

void Model::getThermoMeat1SetTemp(char* data) {
}

void Model::getThermoMeat2SetTemp(char* data) {
}

uint32_t Model::getThermoRemindTime(uint8_t sensor_index) {
    // This represents the change in temperature per second
    float time_rate = 0.0f;
    return 0;
}

void Model::setThermoMeat1SetTime(char* time) {

}

void Model::setThermoMeat2SetTime(char* time) {

}

void Model::reset() {
}

void Model::update() {
    Monitoring::readTemperature_probe1();
    Monitoring::readTemperature_probe2();
    Monitoring::readInternalTemperature();

    if (bme280_tick >= BME280_UPDATE_PERIOD_TICK) {
        bme280.readAll();
        temperature_average.addValue(bme280.getTemperature());
        bme280_tick = 0;
    }

    bme280_tick++;
}

float Model::getBME280Temperature() {
    if (temperature_average.isFull()) {
        return temperature_average.getAverage() / 100.0f;
    }
    return 273000.0f; // Return absolute zero if not enough data
}