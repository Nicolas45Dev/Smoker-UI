#include "MCP23017.hpp"

MCP23017::MCP23017(uint8_t address) : _address(address) {
    spi = SPI::getInstance();

    // Setup CS pin
    gpio_set_direction((gpio_num_t)CS_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)CS_PIN, 1); // Set CS high

    // Setup reset pin
    gpio_set_direction((gpio_num_t)RESET_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)RESET_PIN, 1); // Set reset high
}

void MCP23017::begin() {
    spi->init();

    gpio_set_level((gpio_num_t)RESET_PIN, 1); // Set reset low
}

void MCP23017::pinMode(uint8_t pin, uint8_t mode) {
    uint8_t iodir_reg = pin > 50 ? IODIRA : IODIRB;
    uint8_t pin_mask = pin > 50 ? (1 << (pin - 50)) : (1 << (pin - 40));

    uint8_t iodir = 0;
    uint8_t reg[3] = {(OPCODE | 0x01), iodir_reg, 0};
    iodir = (mode == 0) ? (iodir & ~pin_mask) : (iodir | pin_mask); // Set or clear the bit
    reg[2] = 0;

    // Read current IODIR register
    gpio_set_level((gpio_num_t)CS_PIN, 0);
    spi->writeRead(reg, 3, reg, 1, -1);
    gpio_set_level((gpio_num_t)CS_PIN, 1);

    if (!mode)   
        iodir = reg[2] | pin_mask;
    else
        iodir = reg[2] & ~pin_mask;

    reg[0] = (OPCODE & ~0x01); // Set to write mode
    reg[1] = iodir_reg;
    reg[2] = iodir;

    gpio_set_level((gpio_num_t)CS_PIN, 0);
    spi->write(reg, 3, -1);
    gpio_set_level((gpio_num_t)CS_PIN, 1);
}

void MCP23017::digitalWrite(uint8_t pin, uint8_t value) {
    uint8_t gpio_reg = (pin > 50) ? GPIOA : GPIOB;
    uint8_t pin_mask = pin > 50 ? (1 << (pin - 50)) : (1 << (pin - 40));

    uint8_t gpio = 0;
    uint8_t reg[3] = {(OPCODE | 0x01), gpio_reg, 0};
    gpio = (value == 0) ? (gpio & ~pin_mask) : (gpio | pin_mask); // Set or clear the bit
    reg[2] = 0;

    // Read current IODIR register
    gpio_set_level((gpio_num_t)CS_PIN, 0);
    spi->writeRead(reg, 3, reg, 1, -1);
    gpio_set_level((gpio_num_t)CS_PIN, 1);

    if (value == 0)   
        gpio = reg[2] & ~pin_mask;
    else
        gpio = reg[2] | pin_mask;

    reg[0] = (OPCODE & ~0x01); // Set to write mode
    reg[1] = gpio_reg;
    reg[2] = gpio;

    gpio_set_level((gpio_num_t)CS_PIN, 0);
    spi->write(reg, 3, -1);
    gpio_set_level((gpio_num_t)CS_PIN, 1);
}

int MCP23017::digitalRead(uint8_t pin) {
    uint8_t gpio_reg = (pin > 50) ? GPIOA : GPIOB;
    uint8_t pin_mask = pin > 50 ? (1 << (pin - 50)) : (1 << (pin - 40));

    printf("Reading from pin %d\n", pin);

    uint8_t gpio;
    spi->read(&gpio, 1, CS_PIN);
    return (gpio & pin_mask) ? 1 : 0;
}