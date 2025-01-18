#include "Model.hpp"

Model* Model::_instance = NULL;

Model::Model() : thermo_tank(MAX31855(THERMO_TANK_CS, THERMO_TANK_ACTIVE_LED)), thermo_meat1(MAX31855(THERMO_MEAT1_CS, THERMO_MEAT1_ACTIVE_LED)), thermo_meat2(MAX31855(THERMO_MEAT2_CS, THERMO_MEAT2_ACTIVE_LED))
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
                thermo_remind_time_1 = (temp - previous_thermo_temp_1) * 720;
                previous_thermo_temp_1 = temp;
            } 
            break;
        case 1:
            if(thermo_meat2.isProbeConnected()) {
                temp = thermo_meat2.readTemperature(user_unit);
                thermo_remind_time_2 = (temp - previous_thermo_temp_2) * 720;
                previous_thermo_temp_2 = temp;
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

uint32_t Model::getThermoRemindTime(uint8_t sensor_index) {
    // This represents the change in temperature per second
    float time_rate = 0.0f;
    float temp = 0.0f;
    uint32_t time = 0;

    switch (sensor_index) {
        case 0:
            temp = readThermocouples(0);
            time_rate = (float)(temp - previous_thermo_temp_1) / (2.0f); // 2 is the rate of new reading
            previous_thermo_temp_1 = temp; 
            time = (thermo_meat1_set_temp + EPSILON) / time_rate;
            break;
        case 1:
            temp = readThermocouples(1);
            time_rate = (float)(temp - previous_thermo_temp_2) / (2.0f); // 2 is the rate of new reading //TODO: Add a constant for the rate
            previous_thermo_temp_2 = temp;
            time = (thermo_meat2_set_temp + EPSILON) / time_rate;
            break;
        default:
            time = 0;
            break;
    }

    return time;
}

void Model::setThermoMeat1SetTime(char* time) {
    if(thermo_meat1.isProbeConnected()) {
        uint32_t time_value = getThermoRemindTime(0);
        // total in minutes
        time_value *= 0.01666;
        uint8_t hours = time_value / 60;
        uint8_t minutes = time_value % 60;
        
        sprintf(time, "%d:%d", hours, minutes);
    }
    else {
        sprintf(time, "--");
    }
}

void Model::setThermoMeat2SetTime(char* time) {
    if(thermo_meat2.isProbeConnected()) {
        uint32_t time_value = getThermoRemindTime(1);
        // total in minutes
        time_value /= 60;
        uint8_t hours = time_value / 60;
        uint8_t minutes = time_value % 60;
        
        sprintf(time, "%d:%d", hours, minutes);
    }
    else {
        sprintf(time, "--");
    }
}

void Model::reset() {
    thermo_tank_set_temp = 0.0f;
    thermo_remind_time_1 = 0.0f;
    thermo_remind_time_2 = 0.0f;
}