#ifndef VIEW_HPP
#define VIEW_HPP

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include "pageDrawing.h"
#include "u8g2.h"
#include <u8g2_esp32_hal.hpp>

#define PIN_CLK (gpio_num_t)36
#define PIN_MOSI (gpio_num_t)40
#define PIN_RESET (gpio_num_t)38
#define PIN_CS (gpio_num_t)39

class View
{
private:
    u8g2_t u8g2;
    uint8_t index = 0;
public:
    View(/* args */);
    ~View();
    void initDisplay();
    void drawLogoPage();
    void drawMainPage();
};

#endif // VIEW_HPP