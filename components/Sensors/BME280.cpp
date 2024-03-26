#include "BME280.hpp"

BME280::BME280() {
    spi = SPI::getInstance();
    spi->init();

    // Initialize the gpio pin for the BME280
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << BME280_CS);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(BME280_CS, 1);

    readCalibrationData();

    setCtrlMeas();
}

BME280::~BME280() {
}

void BME280::read() {
    readTemperature();
}

bool BME280::init() {
    return true;
}

float BME280::getTemperature() {
    return temperature;
}

float BME280::getPressure() {
    return 0.0;
}

float BME280::getHumidity() {
    return 0.0;
}

void BME280::readCalibrationData() {
    // Flip the first bit to one
    uint8_t spi_byte = BME280_CALIB_00_REG;
    uint8_t tx_buffer[25];
    tx_buffer[0] = spi_byte;
    uint8_t rx_buffer[25];
    spi->writeRead(tx_buffer, 25, rx_buffer, 25, BME280_CS);

    printf("Calibration data: ");

    // Set the calibration data
    uint16_t temp = (rx_buffer[2] << 8) | rx_buffer[1];
    dig_T1 = temp;

    temp = (rx_buffer[4] << 8) | rx_buffer[3];
    dig_T2 = (int16_t)temp;

    temp = (rx_buffer[6] << 8) | rx_buffer[5];
    dig_T3 = (int16_t)temp;

    uint16_t press = (rx_buffer[8] << 8) | rx_buffer[7];
    dig_P1 = press;

    press = (rx_buffer[10] << 8) | rx_buffer[9];
    dig_P2 = (int16_t)press;

    press = (rx_buffer[12] << 8) | rx_buffer[11];
    dig_P3 = (int16_t)press;

    press = (rx_buffer[14] << 8) | rx_buffer[13];
    dig_P4 = (int16_t)press;

    press = (rx_buffer[16] << 8) | rx_buffer[15];
    dig_P5 = (int16_t)press;

    press = (rx_buffer[18] << 8) | rx_buffer[17];
    dig_P6 = (int16_t)press;

    press = (rx_buffer[20] << 8) | rx_buffer[19];
    dig_P7 = (int16_t)press;

    press = (rx_buffer[22] << 8) | rx_buffer[21];
    dig_P8 = (int16_t)press;

    press = (rx_buffer[24] << 8) | rx_buffer[23];
    dig_P9 = (int16_t)press;

    // dig_H1 = rx_buffer[25];

    // int16_t hum = (rx_buffer[27] << 8) | rx_buffer[26];
    // dig_H2 = (int16_t)hum;

    // dig_H3 = rx_buffer[28];

    // // H4 is the 4 most significant bits of 0x29 and the 4 least significant bits of 0x28
    // int16_t h4 = (rx_buffer[29] << 4) | (rx_buffer[30] & 0x0F);
    // dig_H4 = (int16_t)h4;

    // // H5 is the 4 most significant bits of 0x30 and the 0x31
    // int16_t h5 = (rx_buffer[31] << 4) | (rx_buffer[32] >> 4);
    // dig_H5 = (int16_t)h5;
}

void BME280::readTemperature() {
    uint8_t spi_byte = BME280_TEMP_MSB_REG;
    uint8_t tx_buffer[3] = {0, 0, 0};
    tx_buffer[0] = setBit(true, spi_byte);
    uint8_t rx_buffer[3];

    spi->writeRead(tx_buffer, 3, rx_buffer, 3, BME280_CS);

    uint32_t adc_T = (rx_buffer[1] << 12) | (rx_buffer[2] << 4) | (rx_buffer[3]);

    int32_t var1, var2;
    int32_t t_fine;

    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;

    temperature = (float)((t_fine * 5 + 128) >> 8) / 100;
}

void BME280::readPressure() {
}

void BME280::readHumidity() {
}

uint8_t BME280::setBit(bool read, uint8_t reg) {
    // If read is true, set the first bit to 1
    if(read) {
        return reg | 0x80;
    }
    else {
        return reg ^ 0x80;
    }
}

void BME280::setCtrlMeas() {
    uint8_t spi_byte = BME280_CTRL_MEAS_REG;
    uint8_t tx_buffer[2] = {0, 0};
    tx_buffer[0] = setBit(false, spi_byte);
    tx_buffer[1] = 0x27;
    spi->write(tx_buffer, 2, BME280_CS);
}

void BME280::setCtrlHum() {
    uint8_t spi_byte = BME280_CTRL_HUM_REG;
    uint8_t tx_buffer[2] = {0, 0};
    tx_buffer[0] = setBit(false, spi_byte);
    tx_buffer[1] = 0x01;
    spi->write(tx_buffer, 2, BME280_CS);
}