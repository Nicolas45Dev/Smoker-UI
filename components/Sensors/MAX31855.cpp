#include <MAX31855.hpp>

MAX31855::MAX31855(gpio_num_t pin_cs, gpio_num_t active_led) {
    spi = SPI::getInstance();
    spi->init();
    this->pin_cs = pin_cs;
    this->active_led = active_led;

    // Initialize the gpio pin for the MAX31855
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << pin_cs);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(pin_cs, 1);

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << active_led);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level(active_led, 0);
}

MAX31855::~MAX31855() {
}

bool MAX31855::isProbeConnected() {
    uint8_t tx_buffer[4];
    tx_buffer[0] = 0x00;
    uint8_t rx_buffer[4];
    spi->read(rx_buffer, 4, pin_cs);

    uint32_t data32 = (rx_buffer[0] << 24) | (rx_buffer[1] << 16) | (rx_buffer[2] << 8) | rx_buffer[3];

    // if the probe is connected active the led
    if(!(data32 & OPEN_CIRCUIT_FAULT)) {
        gpio_set_level(active_led, 1);
    } else {
        gpio_set_level(active_led, 0);
    }

    return !(data32 & OPEN_CIRCUIT_FAULT);
}

float MAX31855::readTemperature(TEMP_UNIT unit) {
    uint8_t tx_buffer[4];
    tx_buffer[0] = 0x00;
    uint8_t rx_buffer[4];
    spi->read(rx_buffer, 4, pin_cs);

    uint32_t data32 = (rx_buffer[0] << 24) | (rx_buffer[1] << 16) | (rx_buffer[2] << 8) | rx_buffer[3];

    temperature_probe = (float)(data32 >> 18) * 0.25;

    temperature[avg_index] = temperature_probe;
    avg_index = (avg_index + 1) % 5;

    avg_temperature = (temperature[0] + temperature[1] + temperature[2] + temperature[3] + temperature[4]) / 5;

    // Convert the temperature to the desired unit
    switch(unit) {
        case TEMP_UNIT::CELSIUS_UNIT:
            return avg_temperature;
        case TEMP_UNIT::FAHRENHEIT_UNIT:
            return CELSIUS_TO_FAHRENHEIT(avg_temperature);
        case TEMP_UNIT::KELVIN_UNIT:
            return CELSIUS_TO_KELVIN(avg_temperature);
        default:
            return avg_temperature;
    }
}