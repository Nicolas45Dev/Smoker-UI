#include "View.hpp"

View::View() {
}

View::~View() {
}

void View::initDisplay() {
    ESP_LOGI("View", "Initializing display");
    u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    u8g2_esp32_hal.bus.spi.clk = PIN_CLK;
    u8g2_esp32_hal.bus.spi.mosi = PIN_MOSI;
    u8g2_esp32_hal.bus.spi.cs = PIN_CS;
    u8g2_esp32_hal.reset = PIN_RESET;
    u8g2_esp32_hal_init(u8g2_esp32_hal);
    ESP_LOGI("View", "Display initialized");

    u8g2_Setup_st7920_s_128x64_f(
        &u8g2,
        U8G2_R0,
        u8g2_esp32_spi_byte_cb,
        u8g2_esp32_gpio_and_delay_cb);  // init u8g2 structure

    ESP_LOGI("View", "Setting up display");

    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
}

void View::drawLogoPage() {
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawBitmap(&u8g2, 32, -8, 8, 64, bitmapham_leg_svgrepo_com);
    u8g2_SendBuffer(&u8g2);
}

void View::drawMainPage() {
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawBitmap(&u8g2, 0, 10, 11, 46, tank);
    // u8g2_DrawBitmap(&u8g2, 82, 12, 8, 8, thermometer);
    // u8g2_DrawBitmap(&u8g2, 82, 36, 8, 8, thermometer);
    // u8g2_DrawBitmap(&u8g2, 82, 24, 8, 8, time_icon);
    // u8g2_DrawBitmap(&u8g2, 82, 48, 8, 8, time_icon);
    u8g2_SendBuffer(&u8g2);
}