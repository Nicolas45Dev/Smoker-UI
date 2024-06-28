#include "Model.hpp"

Model::Model() : thermo_tank(MAX31855(THERMO_TANK_CS, THERMO_TANK_ACTIVE_LED)), thermo_meat1(MAX31855(THERMO_MEAT1_CS, THERMO_MEAT1_ACTIVE_LED)), thermo_meat2(MAX31855(THERMO_MEAT2_CS, THERMO_MEAT2_ACTIVE_LED))
{
}

Model::~Model() {
}

// TODO: Fix undeifned reference to Model::getInstance()
Model* Model::getInstance() {
    if(_instance == NULL) {
        _instance = new Model();
    }
    return _instance;
}

void Model::readThermocouples(char* data, uint8_t sensor_index) {
    float temp = 0;
    switch (sensor_index) {
        case 0:
            if(thermo_tank.isProbeConnected()) {
                temp = thermo_tank.readTemperature(user_unit);
                sprintf(data, "%.1f", temp);
            } 
            else {
                sprintf(data, "NC");
            }
            break;
        case 1:
            if(thermo_meat1.isProbeConnected()) {
                temp = thermo_meat1.readTemperature(user_unit);
                sprintf(data, "%.1f", temp);
            } 
            else {
                sprintf(data, "NC");
            }
            break;
        case 2:
            if(thermo_meat2.isProbeConnected()) {
                temp = thermo_meat2.readTemperature(user_unit);
                sprintf(data, "%.1f", temp);
            } 
            else {
                sprintf(data, "NC");
            }
            break;
        default:
            sprintf(data, "NC");
            break;
    }
}

float Model::readThermocouples(uint8_t sensor_index) {
    float temp = 0;
    switch (sensor_index) {
        case 0:
            if(thermo_meat1.isProbeConnected()) {
                temp = thermo_meat1.readTemperature(user_unit);
            } 
            break;
        case 1:
            if(thermo_meat2.isProbeConnected()) {
                temp = thermo_meat2.readTemperature(user_unit);
            } 
            break;
        default:
            temp = 0;
            break;
    }
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
    sprintf(data, "%.1f", thermo_meat1_set_temp);
}

void Model::getThermoMeat2SetTemp(char* data) {
    sprintf(data, "%.1f", thermo_meat2_set_temp);
}