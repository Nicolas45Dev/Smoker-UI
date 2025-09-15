#include "Model.hpp"

Model* Model::_instance = NULL;

Model::Model()
{
    Monitoring::init();
}

Model::~Model() {
}

Model* Model::getInstance() {
    if(_instance == NULL) {
        _instance = new Model();
    }
    return _instance;
}

void Model::readThermometers(char* data, uint8_t sensor_index) {
    Monitoring::MilliCelsius temp = 0;
    switch (sensor_index) {
        case 0:
            temp = Monitoring::readTemperature_probe1();
            break;
        case 1:
            temp = Monitoring::readTemperature_probe2();
            break;
        case 2:
            temp = Monitoring::readInternalTemperature();
            break;
        default:
            temp = 0;
            break;
    }
}

float Model::readThermometers(uint8_t sensor_index) {
    float temp = 0;
    return temp;
}

void Model::readBME280() {
    BME280_Device::bme280.readAll();
}

void Model::setPageChange(bool change, int8_t option_change) {
    //page_change = change;
    page_index++;
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
    thermo_tank_set_temp = 0.0f;
    thermo_remind_time_1 = 0.0f;
    thermo_remind_time_2 = 0.0f;
}