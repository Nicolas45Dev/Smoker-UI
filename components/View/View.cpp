#include "View.hpp"

View::View() {
}

View::~View() {
}

void View::initDisplay() {
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.bus.spi.clk = PIN_CLK;
    u8g2_esp32_hal.bus.spi.mosi = PIN_MOSI;
    u8g2_esp32_hal.bus.spi.cs = PIN_CS;
    u8g2_esp32_hal.reset = PIN_RESET;
    u8g2_esp32_hal_init(u8g2_esp32_hal);

    u8g2_Setup_st7920_s_128x64_f(
        &u8g2,
        U8G2_R0,
        u8g2_esp32_spi_byte_cb,
        u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure

    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,

    u8g2_SetPowerSave(&u8g2, 0); // wake up display
}

void View::drawLogoPage() {
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawXBMP(&u8g2, 32, 0, 8, 64, bitmapham_leg_svgrepo_com);
    u8g2_SendBuffer(&u8g2);
}

void View::drawMainPage() {
    u8g2_ClearBuffer(&u8g2);
    u8g2_SendBuffer(&u8g2);
}