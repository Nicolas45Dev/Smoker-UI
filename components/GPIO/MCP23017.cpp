#include "MCP23017.hpp"
#include <esp_log.h>

static const char* TAG = "MCP23017";

MCP23017::MCP23017(uint8_t address) : _address(address) {
    spi = SPI::getInstance();

    // Setup CS pin
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << CS_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    esp_err_t err = gpio_config(&io_conf);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure CS pin %d as output: %s", static_cast<int>(CS_PIN), esp_err_to_name(err));
    } else {
        err = gpio_set_level(CS_PIN, 1);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to drive CS pin %d high: %s", static_cast<int>(CS_PIN), esp_err_to_name(err));
        }
    }

    // Setup reset pin
    gpio_config_t io_conf_reset;
    io_conf_reset.intr_type = GPIO_INTR_DISABLE;
    io_conf_reset.mode = GPIO_MODE_OUTPUT;
    io_conf_reset.pin_bit_mask = (1ULL << RESET_PIN);
    io_conf_reset.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf_reset.pull_up_en = GPIO_PULLUP_DISABLE;
    err = gpio_config(&io_conf_reset);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure reset pin %d as output: %s", static_cast<int>(RESET_PIN), esp_err_to_name(err));
    } else {
        err = gpio_set_level(RESET_PIN, 1);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Failed to drive reset pin %d high: %s", static_cast<int>(RESET_PIN), esp_err_to_name(err));
        }
    }
}

void MCP23017::begin() {
    spi->init();

    // Reset the MCP23017
    gpio_set_level(RESET_PIN, 1); // Set reset low
    vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to ensure reset
    gpio_set_level(RESET_PIN, 0); // Set reset high
    vTaskDelay(10 / portTICK_PERIOD_MS);
    gpio_set_level(RESET_PIN, 1); // Ensure reset is high
}

void MCP23017::pinMode(uint8_t pin, uint8_t mode) {
    uint8_t iodir_reg = pin > 50 ? IODIRA : IODIRB;
    uint8_t pin_mask = pin > 50 ? (1 << (pin - 50)) : (1 << (pin - 40));

    uint8_t iodir = 0;
    uint8_t reg[3] = {(OPCODE | 0x01), iodir_reg, 0};
    iodir = (mode == 0) ? (iodir & ~pin_mask) : (iodir | pin_mask); // Set or clear the bit
    reg[2] = 0;

    // Read current IODIR register
    gpio_set_level(CS_PIN, 0);
    spi->writeRead(reg, 3, reg, 1, -1);
    gpio_set_level(CS_PIN, 1);

    if (!mode)   
        iodir = reg[2] | pin_mask;
    else
        iodir = reg[2] & ~pin_mask;

    reg[0] = (OPCODE & ~0x01); // Set to write mode
    reg[1] = iodir_reg;
    reg[2] = iodir;

    gpio_set_level(CS_PIN, 0);
    spi->write(reg, 3, -1);
    gpio_set_level(CS_PIN, 1);
}

void MCP23017::digitalWrite(uint8_t pin, uint8_t value) {
    uint8_t gpio_reg = (pin > 50) ? GPIOA : GPIOB;
    uint8_t pin_mask = pin > 50 ? (1 << (pin - 50)) : (1 << (pin - 40));

    uint8_t gpio = 0;
    uint8_t reg[3] = {(OPCODE | 0x01), gpio_reg, 0};
    gpio = (value == 0) ? (gpio & ~pin_mask) : (gpio | pin_mask); // Set or clear the bit
    reg[2] = 0;

    // Read current IODIR register
    gpio_set_level(CS_PIN, 0);
    spi->writeRead(reg, 3, reg, 1, -1);
    gpio_set_level(CS_PIN, 1);

    if (value == 0)   
        gpio = reg[2] & ~pin_mask;
    else
        gpio = reg[2] | pin_mask;

    reg[0] = (OPCODE & ~0x01); // Set to write mode
    reg[1] = gpio_reg;
    reg[2] = gpio;

    gpio_set_level(CS_PIN, 0);
    spi->write(reg, 3, -1);
    gpio_set_level(CS_PIN, 1);
}

int MCP23017::digitalRead(uint8_t pin) {
    uint8_t gpio_reg = (pin > 50) ? GPIOA : GPIOB;
    uint8_t pin_mask = pin > 50 ? (1 << (pin - 50)) : (1 << (pin - 40));

    uint8_t gpio;
    spi->read(&gpio, 1, CS_PIN);
    return (gpio & pin_mask) ? 1 : 0;
}