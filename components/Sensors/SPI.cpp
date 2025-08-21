#include "SPI.hpp"

SPI* SPI::instance = nullptr;

SPI::SPI() {
    m_spi = new ESP32_HAL_SPI();
}

bool SPI::init() {
    return m_spi->init() == ESP_OK;
}

void SPI::write(uint8_t* data, size_t len, uint8_t cs_pin) {
    m_spi->transmit(data, len, cs_pin);
}

void SPI::read(uint8_t* data, size_t len, uint8_t cs_pin) {
    m_spi->receive(data, len, cs_pin);
}

void SPI::writeRead(const uint8_t* tx_data, size_t tx_len, uint8_t* rx_data, size_t rx_len, uint8_t cs_pin) {
    m_spi->writeRead(tx_data, tx_len, rx_data, rx_len, cs_pin);
}