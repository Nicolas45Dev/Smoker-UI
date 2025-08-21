#ifndef SPI_HPP
#define SPI_HPP

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include "esp32_hal_spi.hpp"
#include <esp_log.h>
#include <stdint.h>
#include <string.h>

class SPI
{
private:
    ESP32_HAL_SPI* m_spi;
protected:
    static SPI* instance;
    SPI();
public:
    SPI(SPI&) = delete;
    void operator=(const SPI&) = delete;

    static SPI* getInstance() {
        if(instance == nullptr) {
            instance = new SPI();
            instance->init();
        }
        return instance;
    }

    /**
     * @brief Initialize the SPI bus
     * 
     */
    bool init();

    /**
     * @brief Write data to the SPI bus
     * 
     * @param data 
     * @param len 
     */
    void write(uint8_t* data, size_t len, uint8_t cs_pin);
    
    /**
     * @brief Read data from the SPI bus
     * 
     * @param data 
     * @param len 
     */
    void read(uint8_t* data, size_t len, uint8_t cs_pin);

    void writeRead(const uint8_t* tx_data, size_t tx_len, uint8_t* rx_data = nullptr, size_t rx_len = 0, uint8_t cs_pin = 0);
};


#endif // SPI_HPP