#include "Controller.hpp"

Controller::Controller() {

}

Controller::~Controller() {

}

bool Controller::init() {
    view.initDisplay();
    model = Model::getInstance();

    memset(thermo_tank, 0, sizeof(thermo_tank));
    memset(thermo_meat1, 0, sizeof(thermo_meat1));
    memset(thermo_meat2, 0, sizeof(thermo_meat2));

    page_index = 0;
    start_tick = xTaskGetTickCount();

    m_is_init = !m_is_init;

    return m_is_init;
}

void Controller::run() {
    current_tick++;

    // Check the encoder inputs
    if (Encoder::isOptionChanged()) {
        setOptionChange(Encoder::getOptionChange());
    }

    if (Encoder::isPageChanged()) {
        setPageChange();
    }

    // Let the system init for 2 seconds
    if (current_tick > 20) {
        page_index = 1;
    }

    updateFromModel();

    // Update the view every 5 ticks
    if (current_tick % 5 == 0) {
        updateView();
    }
}

void Controller::updateView() {
    setPageParams(page_index >= 1);

    switch (page_index)
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
            view.drawMeatSelectionPage(option_change);
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

void Controller::updateModel() {
    // update model
}

void Controller::setPageChange() {
    if(current_tick > (previous_tick + 200)) {
        switch (page_index)
        {
        case 0:
            page_index = 1;
            break;
        case 1:
            page_index = 2;
            break;
        case 2:
            setMenuPageFromOption();
            break;
        case 3:
            setMeatProfilePageFromOption();
            break;
        default:
            page_index = 1;
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
        page_index = 3;
        break;
    case 1:
        page_index = 4;
        break;
    case 2: // Close fire
        model->reset();
        page_index = 1;
        break;
    default:
        page_index = 1;
        break;
    }
}

void Controller::setMeatProfilePageFromOption() {
    switch (option_change)
    {
    case 10:
        page_index = 2;
        break;
    default:
        // set meat profile
        model->setThermoTankSetTemp(view.getMeatProfileData(option_change).tank_temp);
        model->setThermoMeat1SetTemp(view.getMeatProfileData(option_change).meat_temp);
        model->setThermoMeat2SetTemp(view.getMeatProfileData(option_change).meat_temp);

        page_index = 1; // Go back to main page

        break;
    }
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
    page_params.set_temp_tank = model->getThermoTankSetTemp();
    page_params.set_temp_meat1 = thermo_meat1_set;
    page_params.set_temp_meat2 = thermo_meat2_set;
    page_params.time_meat1 = time_meat_1;
    page_params.time_meat2 = time_meat_2;
    page_params.selected_option = withOption ? option_change : 0;
    page_params.bme280_data_temp = bme280_temp;
    page_params.bme280_data_press = bme280_press;
}

void Controller::updateFromModel() {
    Monitoring::MilliCelsius temp1 = model->readThermometers(0);
    Monitoring::MilliCelsius temp2 = model->readThermometers(1);
    Monitoring::MilliCelsius temp3 = model->readThermometers(2);

    // Put the temperature in the char array
    if (model->isProbe1Connected()) {
        sprintf(thermo_tank, "%.1f", temp1 / 100.0f);
    } else {
        sprintf(thermo_tank, "----");
    }

    // Read BME280 data
    float bme_temp = model->getBME280Temperature();

    if (bme_temp == 273000.0f) {
        sprintf(bme280_temp, "----");
    } else {
        sprintf(bme280_temp, "%.1fC", bme_temp);
    }
}