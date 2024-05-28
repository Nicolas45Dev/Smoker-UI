#include "Model.hpp"

Model::Model() : thermo_tank(MAX31855(THERMO_TANK_CS, THERMO_TANK_ACTIVE_LED)), thermo_meat1(MAX31855(THERMO_MEAT1_CS, THERMO_MEAT1_ACTIVE_LED)), thermo_meat2(MAX31855(THERMO_MEAT2_CS, THERMO_MEAT2_ACTIVE_LED))
{
}

Model::~Model() {
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

float Model::readBME280(uint8_t sensor_index) {
    float all_values[3] = {bme280.getTemperature(), bme280.getPressure(), bme280.getHumidity()};
    return all_values[sensor_index];
}

void Model::setPageChange(bool change, int8_t option_change) {
    page_change = change;
    page_index++;
}