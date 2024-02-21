#include "Controller.hpp"

Controller::Controller(uint8_t update_interval) {
    view.initDisplay();

    bme280_update_tick = THERMOMETER_UPDATE_INTERVAL / ((float)update_interval / 100.0);
    thermocouple_update_tick = THERMOCOUPLE_UPDATE_INTERVAL / ((float)update_interval / 100.0);

    ESP_LOGI("Controller", "BME280 update tick: %d", bme280_update_tick);
    ESP_LOGI("Controller", "Thermocouple update tick: %d", thermocouple_update_tick);
}

Controller::~Controller() {

}

void Controller::run() {
    time_tick++;

    if (time_tick % bme280_update_tick == 0) {
        // read bme280 data
    }

    if (time_tick % thermocouple_update_tick == 0) {
        // read thermocouple data
    }

    // update view
    updateView();
}

void Controller::readModelData() {
    // read data from model
}

void Controller::updateView() {
    if(time_tick > 20) {
        view.drawMainPage();
    }
    else {
        view.drawLogoPage();
    }
}

void Controller::updateModel() {
    // update model
}