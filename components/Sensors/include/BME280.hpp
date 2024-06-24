#ifndef BME280_HPP
#define BME280_HPP

#include "Config.h"
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <stdint.h>
#include <string.h>
#include <SPI.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define BME280_CS (gpio_num_t)18

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
#define BME280_CALIB_HUM_2_REG 0xE1
#define BME280_CALIB_HUM_1_REG 0xA1
#define BME280_CALIB_HUM_3_REG 0xE3
#define BME280_CALIB_HUM_4_REG 0xE4
#define BME280_CALIB_HUM_5_REG 0xE5

#define BME280_ADDRESS 0x76

class BME280
{
private:
    SPI* spi;

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
    int8_t dig_H6;

    float temperature;
    float pressure;
    float humidity;
    uint32_t t_fine;

    void readTemperature();
    void readPressure();
    void readHumidity();
    void readCalibrationData();
    uint8_t setBit(bool read, uint8_t reg);
    void setCtrlMeas();
    void setCtrlHum();
    void setConfig();
public:
    BME280();
    ~BME280();

    /**
     * @brief Get the Temperature object
     * 
     * @return float 
     */
    float getTemperature(TEMP_UNIT unit = DEFAULT_UNIT);

    /**
     * @brief Get the Pressure object
     * 
     * @return float 
     */
    float getPressure(PERSSURE_UNIT unit = DEFAULT_PRESSURE_UNIT);

    /**
     * @brief Get the Humidity object
     * 
     * @return float 
     */
    float getHumidity();

    void readAll();
};

#endif // BME280_HPP