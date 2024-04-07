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

        printf("Option change: %d\n", option_change);
    }

    // update view
    updateView();
}

void Controller::readModelData() {
    // read all sensors
    
}

void Controller::updateView() {
    if(time_tick < 20) {
        view.drawLogoPage();
    }
    else {
        switch (model.getPageIndex())
        {
        case 0:
            view.drawMainPage(thermo_tank, thermo_meat1, thermo_meat2, model.getThermoTankSetTemp(), model.getThermoMeat1SetTemp(), model.getThermoMeat2SetTemp(), 0, 0);
            break;
        case 1:
            page_option_max = PAGE_OPTION_COUNT[1];
            view.drawMenuPage(option_change);
            break;
        default:
            break;
        }
    }
}

void Controller::updateModel() {
    // update model
}

void Controller::setPageChange() {
    model.setPageChange(true, option_change);
}

void Controller::setOptionChange(bool direction) {
    if(time_tick > (previous_tick + 2)) {
        option_change += direction ? 1 : -1;
        option_change %= page_option_max;
        if(option_change < 0) {
            option_change = page_option_max - 1;
        }
    }
    previous_tick = time_tick;
}