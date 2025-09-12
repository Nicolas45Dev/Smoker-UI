#include "Model.hpp"

Model* Model::_instance = NULL;

Model::Model()
{
}

Model::~Model() {
}

Model* Model::getInstance() {
    if(_instance == NULL) {
        _instance = new Model();
    }
    return _instance;
}

void Model::readThermocouples(char* data, uint8_t sensor_index) {
    float temp = 0;
}

float Model::readThermocouples(uint8_t sensor_index) {
    float temp = 0;
    return temp;
}

void Model::readBME280(uint8_t sensor_index, char* data) {
    bme280.readAll();

    float all_values[3] = {bme280.getTemperature(), bme280.getPressure(), bme280.getHumidity()};

    sprintf(data, "%.1f", all_values[sensor_index]);
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