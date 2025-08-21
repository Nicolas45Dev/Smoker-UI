#ifndef ESP32_HAL_SPI_HPP
#define ESP32_HAL_SPI_HPP

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_err.h>
#include <stdint.h>
#include <string.h>

#define PIN_CLK (gpio_num_t)36
#define PIN_MOSI (gpio_num_t)37
#define PIN_MISO (gpio_num_t)38
#define CLK_SPEED 1 * 1000 * 1000

#define HOST_SPI SPI3_HOST

class ESP32_HAL_SPI {
public:
    ESP32_HAL_SPI(gpio_num_t pin_clk = PIN_CLK, gpio_num_t pin_mosi = PIN_MOSI, gpio_num_t pin_miso = PIN_MISO, uint32_t clk_speed = CLK_SPEED);
    ~ESP32_HAL_SPI();

    esp_err_t init();
    esp_err_t transmit(const uint8_t* data, size_t length, uint8_t cs_pin);
    esp_err_t receive(uint8_t* data, size_t length, uint8_t cs_pin);
    esp_err_t writeRead(const uint8_t* tx_data, size_t tx_length, uint8_t* rx_data = nullptr, size_t rx_length = 0, uint8_t cs_pin = 0);

private:
    bool is_init = false;
    gpio_num_t m_pin_clk;
    gpio_num_t m_pin_mosi;
    gpio_num_t m_pin_miso;
    uint32_t m_clk_speed;

    spi_transaction_t m_transaction;

    spi_bus_config_t m_buscfg;
    spi_device_interface_config_t m_devcfg;
    spi_device_handle_t m_spi;
};

#endif // ESP32_HAL_SPI_HPP