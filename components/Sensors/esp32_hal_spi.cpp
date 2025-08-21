#include "esp32_hal_spi.hpp"

ESP32_HAL_SPI::ESP32_HAL_SPI(gpio_num_t pin_clk, gpio_num_t pin_mosi, gpio_num_t pin_miso, uint32_t clk_speed) {
    this->m_pin_clk = pin_clk;
    this->m_pin_mosi = pin_mosi;
    this->m_pin_miso = pin_miso;
    this->m_clk_speed = clk_speed;

    this->m_buscfg = {
        .mosi_io_num = this->m_pin_mosi,
        .miso_io_num = this->m_pin_miso,
        .sclk_io_num = this->m_pin_clk,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };

    this->m_devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .mode = 0,
        .duty_cycle_pos = 0,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 0,
        .clock_speed_hz = (int)this->m_clk_speed,
        .input_delay_ns = 0,
        .queue_size = 8,
    };
}

ESP32_HAL_SPI::~ESP32_HAL_SPI() {
    if(this->is_init) {
        spi_bus_remove_device(this->m_spi);
        spi_bus_free(HOST_SPI);
    }
}

esp_err_t ESP32_HAL_SPI::init() {
    if(this->is_init) {
        return ESP_OK;
    }

    // Initialize the SPI bus
    esp_err_t ret = spi_bus_initialize(HOST_SPI, &this->m_buscfg, SPI_DMA_CH_AUTO);
    if(ret != ESP_OK) {
        return ret;
    }

    ret = spi_bus_add_device(HOST_SPI, &this->m_devcfg, &this->m_spi);
    if(ret != ESP_OK) {
        spi_bus_free(HOST_SPI);
        return ret;
    }

    this->is_init = true;
    return ESP_OK;
}

esp_err_t ESP32_HAL_SPI::transmit(const uint8_t* data, size_t length, uint8_t cs_pin) {
    if(!this->is_init) {
        return ESP_ERR_INVALID_STATE;
    }

    memset(&this->m_transaction, 0, sizeof(this->m_transaction));
    this->m_transaction.length = length * 8; // Length in bits
    this->m_transaction.tx_buffer = data;

    gpio_set_level((gpio_num_t)cs_pin, 0); // Select the device
    esp_err_t ret = spi_device_transmit(this->m_spi, &this->m_transaction);
    gpio_set_level((gpio_num_t)cs_pin, 1); // Deselect the device

    return ret;
}

esp_err_t ESP32_HAL_SPI::receive(uint8_t* data, size_t length, uint8_t cs_pin) {
    if(!this->is_init) {
        return ESP_ERR_INVALID_STATE;
    }

    memset(&this->m_transaction, 0, sizeof(this->m_transaction));
    this->m_transaction.length = length * 8; // Length in bits
    this->m_transaction.rx_buffer = data;

    gpio_set_level((gpio_num_t)cs_pin, 0); // Select the device
    esp_err_t ret = spi_device_transmit(this->m_spi, &this->m_transaction);
    gpio_set_level((gpio_num_t)cs_pin, 1); // Deselect the device
    return ret;
}

esp_err_t ESP32_HAL_SPI::writeRead(const uint8_t* tx_data, size_t tx_length, uint8_t* rx_data, size_t rx_length, uint8_t cs_pin) {
    memset(&this->m_transaction, 0, sizeof(this->m_transaction));
    this->m_transaction.length = (tx_length) * 8; // Length in bits
    this->m_transaction.tx_buffer = tx_data;
    this->m_transaction.rx_buffer = rx_data;

    gpio_set_level((gpio_num_t)cs_pin, 0); // Select the device
    esp_err_t ret = spi_device_transmit(this->m_spi, &this->m_transaction);
    gpio_set_level((gpio_num_t)cs_pin, 1); // Deselect the device

    return ret;
}