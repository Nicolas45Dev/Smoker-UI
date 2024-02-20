#include "BME280.hpp"

BME280::BME280() {
}

BME280::~BME280() {
}

void BME280::read() {
}

bool BME280::init() {
    return true;
}

float BME280::getTemperature() {
    return 0.0;
}

float BME280::getPressure() {
    return 0.0;
}

float BME280::getHumidity() {
    return 0.0;
}

void BME280::readCalibrationData() {
    // Flip the first bit to one
    uint8_t spi_byte = BME280_CALIB_00_REG ^ 0x80;
    uint8_t tx_buffer[20];

    SPI::getInstance().writeRead(tx_buffer, 20, &spi_byte, 1);

    // dig_T1 = (data[0] >> 8) | (data[1] << 8);
    // dig_T2 = (int16_t)(data[0] >> 8) | (data[1] << 8);
    // dig_T3 = (int16_t)(data[0] >> 8) | (data[1] << 8);
}

void BME280::readTemperature() {
}

void BME280::readPressure() {
}

void BME280::readHumidity() {
}
