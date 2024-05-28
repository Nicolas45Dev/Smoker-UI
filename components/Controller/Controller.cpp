#include "Controller.hpp"

Controller::Controller(uint8_t update_interval) {
    view.initDisplay();

    memset(thermo_tank, 0, sizeof(thermo_tank));
    memset(thermo_meat1, 0, sizeof(thermo_meat1));
    memset(thermo_meat2, 0, sizeof(thermo_meat2));

    start_tick = xTaskGetTickCount();
}

Controller::~Controller() {

}

void Controller::run() {
    uint32_t current_tick = xTaskGetTickCount();

    if(current_tick - start_tick >= 2000) {
        model.setPageIndex(1);
    }

    if (current_tick % THERMOCOUPLE_UPDATE_INTERVAL == 0) {
        model.readThermocouples(thermo_tank, 0);
        model.readThermocouples(thermo_meat1, 1);
        model.readThermocouples(thermo_meat2, 2);
    }

    if (current_tick % THERMOMETER_UPDATE_INTERVAL == 0) {
        readModelData();
    }

    // update view
    updateView();
}

void Controller::readModelData() {
    // read all sensors
    
}

void Controller::updateView() {

    setPageParams(model.getPageIndex() == 2);
    if(!is_active) {
        model.setPageIndex(5);
    }

    switch (model.getPageIndex())
    {
        case 0:
            view.drawLogoPage();
            break;
        case 1:
            page_option_max = PAGE_OPTION_COUNT[0];
            view.drawMainPage(page_params);
            break;
        case 2:
            page_option_max = PAGE_OPTION_COUNT[1];
            view.drawMenuPage(option_change);
            break;
        case 3:
            page_option_max = PAGE_OPTION_COUNT[2];
            view.drawMeatSelectionPage(option_change);
            break;
        case 4:
            page_option_max = PAGE_OPTION_COUNT[3];
            view.drawSettingsPage(option_change);
            break;
        case 5:
            page_option_max = PAGE_OPTION_COUNT[4];
            //view.drawSavePage(option_change);
            break;
        default:
            break;
    }
}

void Controller::updateModel() {
    // update model
}

void Controller::setPageChange() {
    page_option_history[model.getPageIndex()] = option_change;
    option_change = 0;
    model.setPageChange(true, option_change);
}

void Controller::setOptionChange(bool direction) {
    if(current_tick > (previous_tick + 200)) {
        option_change += direction ? 1 : -1;
        option_change %= page_option_max;
        if(option_change < 0) {
            option_change = page_option_max - 1;
        }
    }
    previous_tick = current_tick;
}

void Controller::setPageParams(bool withOption) {
    page_params.temp_tank1 = thermo_tank;
    page_params.temp_meat1 = thermo_meat1;
    page_params.temp_meat2 = thermo_meat2;
    page_params.set_temp_tank = model.getThermoTankSetTemp();
    page_params.set_temp_meat1 = model.getThermoMeat1SetTemp();
    page_params.set_temp_meat2 = model.getThermoMeat2SetTemp();
    page_params.time_meat1 = 0;
    page_params.time_meat2 = 0;
    page_params.bme280_data = bme280_data;
    page_params.selected_option = withOption ? option_change : 0;
}