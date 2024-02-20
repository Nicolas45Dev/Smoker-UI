#ifndef BME280_HPP
#define BME280_HPP

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <stdint.h>
#include <Sensors.hpp>
#include <string.h>
#include <SPI.hpp>

#define PIN_CLK (gpio_num_t)36
#define PIN_MOSI (gpio_num_t)40
#define PIN_RESET (gpio_num_t)38
#define PIN_CS (gpio_num_t)39

#define BME280_HUM_LSB_REG 0xFE
#define BME280_HUM_MSB_REG 0xFD

#define BME280_TEMP_XLSB_REG 0xFC
#define BME280_TEMP_LSB_REG 0xFB
#define BME280_TEMP_MSB_REG 0xFA

#define BME280_PRESS_XLSB_REG 0xF9
#define BME280_PRESS_LSB_REG 0xF8
#define BME280_PRESS_MSB_REG 0xF7

#define BME280_CONFIG_REG 0xF5
#define BME280_CTRL_MEAS_REG 0xF4
#define BME280_STATUS_REG 0xF3
#define BME280_CTRL_HUM_REG 0xF2
#define BME280_RESET_REG 0xE0
#define BME280_ID_REG 0xD0

#define BME280_CALIB_00_REG 0x88
#define BME280_CALIB_26_REG 0xE1

#define BME280_ADDRESS 0x76

class BME280 : public ISensors
{
private:
    spi_device_handle_t spi;

    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;

    float temperature;
    float pressure;
    float humidity;

    void readCalibrationData();
    void readTemperature();
    void readPressure();
    void readHumidity();
public:
    BME280();
    ~BME280();
    void read() override;
    bool init() override;
    float getTemperature();
    float getPressure();
    float getHumidity();
};

#endif // BME280_HPP