#ifndef SPI_HPP
#define SPI_HPP

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <stdint.h>
#include <string.h>

#define PIN_CLK (gpio_num_t)36
#define PIN_MOSI (gpio_num_t)37
#define PIN_MISO (gpio_num_t)38
#define CLK_SPEED 1 * 1000 * 1000

#define HOST_SPI SPI3_HOST

class SPI
{
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

protected:
    SPI(gpio_num_t pin_clk, gpio_num_t pin_mosi, gpio_num_t pin_miso, uint32_t clk_speed);
    static SPI* instance;
public:
    SPI(SPI&) = delete;
    void operator=(const SPI&) = delete;

    static SPI* getInstance() {
        if(instance == nullptr) {
            instance = new SPI(PIN_CLK, PIN_MOSI, PIN_MISO, CLK_SPEED);
        }
        return instance;
    }

    /**
     * @brief Initialize the SPI bus
     * 
     */
    esp_err_t init();

    /**
     * @brief Write data to the SPI bus
     * 
     * @param data 
     * @param len 
     */
    void write(uint8_t* data, size_t len, gpio_num_t pin_cs);
    
    /**
     * @brief Read data from the SPI bus
     * 
     * @param data 
     * @param len 
     */
    void read(uint8_t* data, size_t len, gpio_num_t pin_cs);

    void writeRead(uint8_t* tx_data, size_t tx_len, uint8_t* rx_data, size_t rx_len, gpio_num_t pin_cs);
};


#endif // SPI_HPP