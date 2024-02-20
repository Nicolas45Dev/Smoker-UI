#include "SPI.hpp"

SPI::SPI(gpio_num_t pin_clk, gpio_num_t pin_mosi, gpio_num_t pin_miso, gpio_num_t pin_cs, uint32_t clk_speed) {
    this->m_pin_clk = pin_clk;
    this->m_pin_mosi = pin_mosi;
    this->m_pin_miso = pin_miso;
    this->m_pin_cs = pin_cs;
    this->m_clk_speed = clk_speed;

    this->init();
}

void SPI::init() {
    m_buscfg = {
        .mosi_io_num = this->m_pin_mosi,
        .miso_io_num = this->m_pin_miso,
        .sclk_io_num = this->m_pin_clk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };

    m_devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = (int)this->m_clk_speed,
        .input_delay_ns = 0
    };

    spi_bus_add_device(HOST_SPI, &m_devcfg, &m_spi);
}

void SPI::write(uint8_t* data, size_t len) {
    memset(&this->m_transaction, 0, sizeof(this->m_transaction));
    this->m_transaction.length = len * 8;
    this->m_transaction.tx_buffer = data;
    this->m_transaction.user = (void*)0;

    // Pull the CS pin low to start the transaction
    gpio_set_level(this->m_pin_cs, 0);
    esp_err_t ret = spi_device_transmit(this->m_spi, &this->m_transaction);
    // Pull the CS pin high to end the transaction
    gpio_set_level(this->m_pin_cs, 1);
}

void SPI::read(uint8_t* data, size_t len) {
    memset(&this->m_transaction, 0, sizeof(this->m_transaction));
    this->m_transaction.length = len * 8;
    this->m_transaction.rx_buffer = data;
    this->m_transaction.user = (void*)0;

    // Pull the CS pin low to start the transaction
    gpio_set_level(this->m_pin_cs, 0);
    esp_err_t ret = spi_device_transmit(this->m_spi, &this->m_transaction);
    // Pull the CS pin high to end the transaction
    gpio_set_level(this->m_pin_cs, 1);
}

void SPI::writeRead(uint8_t* tx_data, size_t tx_len, uint8_t* rx_data, size_t rx_len) {
}