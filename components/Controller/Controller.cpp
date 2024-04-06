#include "Controller.hpp"

Controller::Controller(uint8_t update_interval) {
    view.initDisplay();

    bme280_update_tick = THERMOMETER_UPDATE_INTERVAL / ((float)update_interval / 100.0);
    thermocouple_update_tick = THERMOCOUPLE_UPDATE_INTERVAL / ((float)update_interval / 100.0);

    memset(thermo_tank, 0, sizeof(thermo_tank));
    memset(thermo_meat1, 0, sizeof(thermo_meat1));
    memset(thermo_meat2, 0, sizeof(thermo_meat2));
}

Controller::~Controller() {

}

void Controller::run() {
    time_tick++;

    if (time_tick % THERMOCOUPLE_UPDATE_INTERVAL == 0) {
        model.readThermocouples(thermo_tank, 0);
        model.readThermocouples(thermo_meat1, 1);
        model.readThermocouples(thermo_meat2, 2);
    }

    // update view
    updateView();
}

void Controller::readModelData() {
    // read all sensors
    
}

void Controller::updateView() {
    if(time_tick > 20) {
        view.drawMainPage(thermo_tank, thermo_meat1, thermo_meat2, model.getThermoTankSetTemp(), model.getThermoMeat1SetTemp(), model.getThermoMeat2SetTemp(), 0, 0);
    }
    // else if(time_tick >= 50) {
    //     view.drawMenuPage(0);
    // }
    else {
        view.drawLogoPage();
    }
}

void Controller::updateModel() {
    // update model
}