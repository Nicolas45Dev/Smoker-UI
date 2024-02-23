#include "Controller.hpp"

Controller::Controller(uint8_t update_interval) {
    view.initDisplay();

    bme280_update_tick = THERMOMETER_UPDATE_INTERVAL / ((float)update_interval / 100.0);
    thermocouple_update_tick = THERMOCOUPLE_UPDATE_INTERVAL / ((float)update_interval / 100.0);

    //ESP_LOGI("Controller", "BME280 update tick: %d", bme280_update_tick);
    //ESP_LOGI("Controller", "Thermocouple update tick: %d", thermocouple_update_tick);
}

Controller::~Controller() {

}

void Controller::run() {
    time_tick++;

    // update view
    updateView();
}

void Controller::readModelData() {
    // read data from model
    model.readAllSensors();
}

void Controller::updateView() {
    if(time_tick > 20 && time_tick < 50) {
        view.drawMainPage(120, 67, 34, 45, 67, 23, 3, 4);
    }
    else if(time_tick >= 50) {
        view.drawMenuPage(0);
    }
    else {
        view.drawLogoPage();
    }
}

void Controller::updateModel() {
    // update model
}