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
    readPressure();
    readHumidity();
}

bool BME280::init() {
    return true;
}

float BME280::getTemperature() {
    return temperature;
}

float BME280::getPressure() {
    return pressure;
}

float BME280::getHumidity() {
    return humidity;
}

void BME280::readCalibrationData() {
    // Flip the first bit to one
    uint8_t spi_byte = BME280_CALIB_00_REG;
    uint8_t tx_buffer[32];
    tx_buffer[0] = spi_byte;
    uint8_t rx_buffer[32];
    spi->writeRead(tx_buffer, 32, rx_buffer, 32, BME280_CS);

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

    uint8_t spi_byte2 = BME280_CALIB_HUM_1_REG;
    tx_buffer[0] = setBit(true, spi_byte2);
    spi->writeRead(tx_buffer, 2, rx_buffer, 2, BME280_CS);

    dig_H1 = rx_buffer[1];

    spi_byte2 = BME280_CALIB_HUM_2_REG;
    tx_buffer[0] = setBit(true, spi_byte2);
    spi->writeRead(tx_buffer, 8, rx_buffer, 8, BME280_CS);

    dig_H2 = (int16_t)((rx_buffer[2] << 8) | rx_buffer[1]);
    dig_H3 = rx_buffer[3];

    dig_H4 = (int16_t)((rx_buffer[4] << 4) | (rx_buffer[5] & 0x0F));
    dig_H5 = (int16_t)((rx_buffer[6] << 4) | ((rx_buffer[5] >> 4) & 0x0F));
    dig_H6 = (int8_t)rx_buffer[7];

    printf("dig_H1: %d\n", dig_H1);
    printf("dig_H2: %d\n", dig_H2);
    printf("dig_H3: %d\n", dig_H3);
    printf("dig_H4: %d\n", dig_H4);
    printf("dig_H5: %d\n", dig_H5);
    printf("dig_H6: %d\n", dig_H6);

}

void BME280::readTemperature() {
    uint8_t spi_byte = BME280_TEMP_MSB_REG;
    uint8_t tx_buffer[3] = {0, 0, 0};
    tx_buffer[0] = setBit(true, spi_byte);
    uint8_t rx_buffer[4];

    spi->writeRead(tx_buffer, 3, rx_buffer, 3, BME280_CS);

    uint32_t adc_T = (rx_buffer[1] << 12) | (rx_buffer[2] << 4) | (rx_buffer[3]);

    int32_t var1, var2;

    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    printf("t_fine: %lu\n", t_fine);

    temperature = (float)((t_fine * 5 + 128) >> 8) / 100;
}

void BME280::readPressure() {
    uint8_t spi_byte = BME280_PRESS_MSB_REG;
    uint8_t tx_buffer[3] = {0, 0, 0};
    tx_buffer[0] = setBit(true, spi_byte);
    uint8_t rx_buffer[4];

    spi->writeRead(tx_buffer, 3, rx_buffer, 3, BME280_CS);

    uint32_t adc_P = (rx_buffer[1] << 12) | (rx_buffer[2] << 4) | (rx_buffer[3]);

    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    printf("t_fine: %lu\n", t_fine);
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;

    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);

    pressure = (float)p / 256.0;
}

void BME280::readHumidity() {
    uint8_t spi_byte = BME280_HUM_MSB_REG;
    uint8_t tx_buffer[3] = {0, 0, 0};
    tx_buffer[0] = setBit(true, spi_byte);
    uint8_t rx_buffer[4];

    spi->writeRead(tx_buffer, 3, rx_buffer, 3, BME280_CS);

    uint32_t adc_H = (rx_buffer[1] << 8) | rx_buffer[2];

    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);

    humidity = (float)(v_x1_u32r >> 12) / 1024.0;
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