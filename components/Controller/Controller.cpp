#include "Controller.hpp"

Controller::Controller(const uint8_t update_interval) {
    view.initDisplay();

    memset(thermo_tank, 0, sizeof(thermo_tank));
    memset(thermo_meat1, 0, sizeof(thermo_meat1));
    memset(thermo_meat2, 0, sizeof(thermo_meat2));

    start_tick = xTaskGetTickCount();
    thermocouple_update_tick = start_tick;
    bme280_update_tick = start_tick;
}

Controller::~Controller() = default;

void Controller::run() {
    current_tick = xTaskGetTickCount();

    if(current_tick - start_tick >= 2000 && starting) {
        starting = false;
        model.setPageIndex(1);
    }

    if (current_tick - thermocouple_update_tick >= THERMOCOUPLE_UPDATE_INTERVAL) {
        model.readThermocouples(thermo_tank, 0);
        model.readThermocouples(thermo_meat1, 1);
        model.readThermocouples(thermo_meat2, 2);

        model.getThermoMeat1SetTemp(thermo_meat1_set);
        model.getThermoMeat2SetTemp(thermo_meat2_set);

        computeRemainingTime();

        thermocouple_update_tick = current_tick;
    }

    if (current_tick - bme280_update_tick >= THERMOMETER_UPDATE_INTERVAL) {
        readModelData();
        bme280_update_tick = current_tick;
    }

    // update view
    updateView();
}

void Controller::computeRemainingTime() {
    const float temp1 = model.readThermocouples(0);
    const float temp2 = model.readThermocouples(1);

    const float delta_temp1 = temp1 - previous_thermo_1;
    const float delta_temp2 = temp2 - previous_thermo_2;

    const float derived_temp_1 = delta_temp1 / (THERMOCOUPLE_UPDATE_INTERVAL / 1000.0);
    const float derived_temp_2 = delta_temp2 / (THERMOCOUPLE_UPDATE_INTERVAL / 1000.0);

    if(derived_temp_1 != 0 && model.getThermoMeat1SetTemp() != 0) {
        time_meat_1 = static_cast<uint32_t>((model.getThermoMeat1SetTemp() - temp1) / derived_temp_1);
    }

    if(derived_temp_2 != 0 && model.getThermoMeat2SetTemp() != 0) {
        time_meat_2 = static_cast<uint32_t>((model.getThermoMeat2SetTemp() - temp2) / derived_temp_2);
    }

    previous_thermo_1 = temp1;
    previous_thermo_2 = temp2;
}

void Controller::readModelData() {
    // read all sensors
    model.readBME280(0, bme280_data_str_temp);
    model.readBME280(2, bme280_data_str_hum);
}

void Controller::updateView() {

    setPageParams(model.getPageIndex() >= 1);
    if(!is_active) {
        model.setPageIndex(5);
    }
    switch (model.getPageIndex())
    {
        case 0:
            view.drawLogoPage();
            break;
        case 1:
            page_option_max = PAGE_OPTION_COUNT[1];
            view.drawMainPage(page_params);
            break;
        case 2:
            page_option_max = PAGE_OPTION_COUNT[2];
            view.drawMenuPage(option_change);
            break;
        case 3:
            page_option_max = PAGE_OPTION_COUNT[3];
            view.drawMeatSelectionPage(option_change, model.getMeatProfile());
            break;
        case 4:
            page_option_max = PAGE_OPTION_COUNT[4];
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

void Controller::setPageChange() {
    if(current_tick > (previous_tick + 200)) {
        switch (model.getPageIndex())
        {
        case 0:
            model.setPageIndex(1);
            break;
        case 1:
            model.setPageIndex(2);
            break;
        case 2:
            setMenuPageFromOption();
            break;
        case 3:
            setMeatProfilePageFromOption();
            break;
        default:
            model.setPageIndex(1);
            break;
        }

        option_change = 0;
    }

    previous_tick = current_tick;
}

void Controller::setMenuPageFromOption() {
    switch (option_change)
    {
    case 0:
        model.setPageIndex(3);
        break;
    case 1:
        model.setPageIndex(4);
        break;
    case 2:
        model.setPageIndex(1);
        break;
    default:
        model.setPageIndex(1);
        break;
    }
}

void Controller::setMeatProfilePageFromOption() {
    switch (option_change)
    {
    case 10:
        model.setPageIndex(2);
        break;
    default:
        // set meat profile
        model.setThermoTankSetTemp(model.getMeatProfile()[option_change].tank_temp);
        model.setThermoMeat1SetTemp(model.getMeatProfile()[option_change].meat_temp);
        model.setThermoMeat2SetTemp(model.getMeatProfile()[option_change].meat_temp);

        model.setPageIndex(1);
        break;
    }
}

void Controller::setOptionChange(const bool direction) {
    if(current_tick > (previous_tick + 200)) {
        option_change += direction ? 1 : -1;
        option_change %= page_option_max;
        if(option_change < 0) {
            option_change = page_option_max - 1;
        }
    }
    previous_tick = current_tick;
}

void Controller::setSettingsPageFromOption() {
    switch (option_change)
    {
    case 0:
        // Units are in Celsius
        model.setUnit(TEMP_UNIT::CELSIUS_UNIT);
        model.setPageIndex(1);
        break;
    case 1:
        // Units are in Fahrenheit
        model.setUnit(TEMP_UNIT::FAHRENHEIT_UNIT);
        model.setPageIndex(1);
        break;
    default:
        model.setPageIndex(1);
        break;
    }
}

void Controller::setPageParams(const bool withOption) {
    page_params.temp_tank1 = thermo_tank;
    page_params.temp_meat1 = thermo_meat1;
    page_params.temp_meat2 = thermo_meat2;
    page_params.set_temp_tank = model.getThermoTankSetTemp();
    page_params.set_temp_meat1 = thermo_meat1_set;
    page_params.set_temp_meat2 = thermo_meat2_set;
    page_params.time_meat1 = time_meat_1;
    page_params.time_meat2 = time_meat_2;
    page_params.bme280_data_temp = bme280_data_str_temp;
    page_params.bme280_data_hum = bme280_data_str_hum;
    page_params.selected_option = withOption ? option_change : 0;
}