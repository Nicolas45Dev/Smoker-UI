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
    u8g2_DrawBitmap(&u8g2, 32, -8, 8, 64, bitmapham_leg_svgrepo_com);
    u8g2_SendBuffer(&u8g2);
}

void View::drawMainPage(char* temp_tank1, char* temp_meat1, char* temp_meat2,
                        float set_temp_tank, float set_temp_meat1, float set_temp_meat2,
                        uint32_t time_meat1, uint32_t time_meat2)
{
    char buffer_t1[10];
    char buffer_t2[10];
    char buffer_t3[10];
    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawBitmap(&u8g2, 0, 10, 10, 46, tank);
    u8g2_DrawBitmap(&u8g2, 82, 12, 1, 8, thermometer);
    u8g2_DrawBitmap(&u8g2, 82, 36, 1, 8, thermometer);
    u8g2_DrawBitmap(&u8g2, 82, 24, 1, 8, time_icon);
    u8g2_DrawBitmap(&u8g2, 82, 48, 1, 8, time_icon);

    u8g2_SetFont(&u8g2, u8g2_font_tinytim_tr);
    u8g2_DrawStr(&u8g2, 32, 7, "Smoker");

    // String for temperature tank
    u8g2_DrawStr(&u8g2, 28, 32, temp_tank1);
    u8g2_DrawStr(&u8g2, 50, 32, "C");

    u8g2_DrawLine(&u8g2, 26, 34, 56, 34);

    // Write the target temperature for the tank under the current temperature
    sprintf(buffer_t1, "%.1f", set_temp_tank);
    u8g2_DrawStr(&u8g2, 28, 42, buffer_t1);
    u8g2_DrawStr(&u8g2, 50, 42, "C");

    // String for temperature 1
    u8g2_DrawStr(&u8g2, 92, 19, temp_meat1);

    // String for temperature 2
    u8g2_DrawStr(&u8g2, 92, 43, temp_meat2);

    // String for time 1
    u8g2_DrawStr(&u8g2, 92, 31, std::to_string(time_meat1).c_str());

    // String for time 2
    u8g2_DrawStr(&u8g2, 92, 55, std::to_string(time_meat2).c_str());

    u8g2_SendBuffer(&u8g2);
}

void View::drawMenuPage(uint8_t selected_option) {
    u8g2_ClearBuffer(&u8g2);
    setPageHeader("Menu");

    if(selected_option < 2) {
        u8g2_DrawButtonUTF8(&u8g2, 8, 20, button_style[selected_option == 0], 0, 2, 2, "Probe 1 : ON/OFF");
        u8g2_DrawButtonUTF8(&u8g2, 8, 36, button_style[selected_option == 1], 0, 2, 2, "Probe 2 : ON/OFF");
    }
    else {
        u8g2_DrawButtonUTF8(&u8g2, 8, 20, button_style[selected_option == 2], 0, 2, 2, "Meat profile");
        u8g2_DrawButtonUTF8(&u8g2, 8, 36, button_style[selected_option == 3], 0, 2, 2, "Settings");
    }

    u8g2_DrawButtonUTF8(&u8g2, 8, 60, button_style[selected_option == 4], 0, 2, 2, "Save");
    u8g2_DrawButtonUTF8(&u8g2, 90, 60, button_style[selected_option == 5], 0, 2, 2, "Exit");

    u8g2_SendBuffer(&u8g2);
}

void View::setPageHeader(const char *header) {
    // Center the text on the display and add * in the beginning and end of the header
    u8g2_DrawStr(&u8g2, 32, 7, header);
}

void View::drawMeatSelectionPage(uint8_t selected_option) {
    u8g2_ClearBuffer(&u8g2);
    setPageHeader("Meat profile");

    for (uint8_t i = 0; i < 2; i++) {
        u8g2_DrawButtonUTF8(&u8g2, 8, 20 + (i * 8), button_style[selected_option % 6], 0, 2, 2, meat_profile_data[i].name);
    }
    
    
    u8g2_DrawButtonUTF8(&u8g2, 90, 60, button_style[selected_option % 5], 0, 2, 2, "Back");

    u8g2_SendBuffer(&u8g2);
}