#ifndef MCP23017_HPP
#define MCP23017_HPP

#include <driver/gpio.h>
#include "SPI.hpp"
#include <stdint.h>

#define MCP23017_ADDRESS 0x00
#define OPCODE 0x40

const uint8_t CS_PIN = 39;
const uint8_t RESET_PIN = 18;

// MCP23017 Register Addresses
#define IODIRA   0x00
#define IODIRB   0x01
#define IPOLA   0x02
#define IPOLB   0x03
#define GPINTENA 0x04
#define GPINTENB 0x05
#define DEFVALA  0x06
#define DEFVALB  0x07
#define INTCONA  0x08
#define INTCONB  0x09
#define IOCON    0x0A
#define GPPUA    0x0C
#define GPPUB    0x0D
#define INTFA    0x0E
#define INTFB    0x0F
#define INTCAPA  0x10
#define INTCAPB  0x11
#define GPIOA    0x12
#define GPIOB    0x13
#define OLATA    0x14
#define OLATB    0x15

class MCP23017 {
public:
    MCP23017(uint8_t address = MCP23017_ADDRESS);
    void begin();
    void pinMode(uint8_t pin, uint8_t mode);
    void digitalWrite(uint8_t pin, uint8_t value);
    int digitalRead(uint8_t pin);

private:
    uint8_t _address;
    SPI *spi;
};

#endif // MCP23017_HPP