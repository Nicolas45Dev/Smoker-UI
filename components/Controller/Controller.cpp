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

    if (time_tick % 20 == 0) {
        //read bme280 data
        bme280.read();
        float temp = bme280.getTemperature();
        float pressure = bme280.getPressure();
        float humidity = bme280.getHumidity();
        printf("Temperature: %f\n", temp);
        printf("Pressure: %f\n", pressure);
        printf("Humidity: %f\n", humidity);
    }

    if (time_tick % THERMOCOUPLE_UPDATE_INTERVAL == 0) {
        // read thermocouple data
        // uint8_t data[4];
        // spi->read(data, 4, GPIO_NUM_19);

        // uint32_t data32 = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
        // if(data32 & 0x07) {
        //     ESP_LOGE("Controller", "Thermocouple data error");
        //     return;
        // }
        // data32 = data32 >> 18;
        // ESP_LOGI("Controller", "Thermocouple data: %lu", data32);
        // float temp = data32 * 0.25;
        // ESP_LOGI("Controller", "Thermocouple temperature: %f", temp);
    }

    // update view
    //updateView();
}

void Controller::readModelData() {
    // read data from model
    //model.readAllSensors();
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