#include "BME280.hpp"

using namespace BME280_Device;

BME280::BME280() {
}

BME280::~BME280() {
}

void BME280::init() {
    spi = SPI::getInstance();

    // Initialize the gpio pin for the BME280
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << BME280_CS);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(BME280_CS, 0);

    readCalibrationData();

    setCtrlMeas();

    gpio_set_level(BME280_CS, 1);
}

Monitoring::MilliCelsius BME280::getTemperature(TEMP_UNIT unit) {
    Monitoring::MilliCelsius new_temperature = 0;

    switch(unit) {
        case FAHRENHEIT_UNIT:
            new_temperature = (Monitoring::MilliCelsius)(CELSIUS_TO_FAHRENHEIT(temperature) * 100);
            break;
        case KELVIN_UNIT:
            new_temperature = (Monitoring::MilliCelsius)(CELSIUS_TO_KELVIN(temperature) * 100);
            break;
        default:
            return (Monitoring::MilliCelsius)temperature * 100;
    }

    return new_temperature;
}

float BME280::getPressure(PERSSURE_UNIT unit) {
    float new_pressure = 0;

    switch(unit) {
        case HPA_UNIT:
            new_pressure = PASCAL_TO_HECTOPASCAL(pressure);
            break;
        case PSI_UNIT:
            new_pressure = PASCAL_TO_PSI(pressure);
            break;
        case BAR_UNIT:
            new_pressure = PASCAL_TO_BAR(pressure);
            break;
        case ATM_UNIT:
            new_pressure = PASCAL_TO_ATMOSPHERE(pressure);
            break;
        default:
            return pressure;
    }

    return new_pressure;
}

void BME280::readCalibrationData() {
    // Flip the first bit to one
    uint8_t spi_byte = BME280_CALIB_00_REG;
    uint8_t tx_buffer[32];
    tx_buffer[0] = spi_byte;
    uint8_t rx_buffer[32];
    spi->writeRead(tx_buffer, 32, rx_buffer, 32, BME280_CS);

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
}

void BME280::readTemperature() {
    uint8_t spi_byte = BME280_TEMP_MSB_REG;
    uint8_t tx_buffer[3] = {0, 0, 0};
    tx_buffer[0] = setBit(true, spi_byte);
    uint8_t rx_buffer[3];

    spi->writeRead(tx_buffer, 3, rx_buffer, 3, BME280_CS);

    uint32_t adc_T = (rx_buffer[1] << 16) | (rx_buffer[2] << 8) | (rx_buffer[3]);
    adc_T >>= 4;

    int32_t var1, var2;

    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;

    temperature = (float)((t_fine * 5 + 128) >> 8) / 100;
}

void BME280::readPressure() {
    uint8_t spi_byte = BME280_PRESS_MSB_REG;
    uint8_t tx_buffer[3] = {0, 0, 0};
    tx_buffer[0] = setBit(true, spi_byte);
    uint8_t rx_buffer[4];

    spi->writeRead(tx_buffer, 3, rx_buffer, 3, BME280_CS);

    uint32_t adc_P = (rx_buffer[1] << 16) | (rx_buffer[2] << 8) | (rx_buffer[3]);
    adc_P >>= 4;

    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
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

void BME280::setConfig() {
    uint8_t spi_byte = BME280_CONFIG_REG;
    uint8_t tx_buffer[2] = {0, 0};
    tx_buffer[0] = setBit(false, spi_byte);
    tx_buffer[1] = 0x00;
    spi->write(tx_buffer, 2, BME280_CS);
}

void BME280::readAll() {
    readTemperature();
    readPressure();
}