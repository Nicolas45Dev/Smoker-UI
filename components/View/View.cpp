#include "View.hpp"

View::View() {
    // Set the meat profile in alphabetical order
    this->meat_profile_data[0] = {0, "Bacon", 65, 120};
    this->meat_profile_data[1] = {1, "Beef", 55, 90};
    this->meat_profile_data[2] = {2, "Chicken", 84, 120};
    this->meat_profile_data[3] = {3, "Cold Smoke", -1, -1};
    this->meat_profile_data[4] = {4, "Fish", 60, 120};
    this->meat_profile_data[5] = {5, "General smoking", -1, 120};
    this->meat_profile_data[6] = {6, "Pork high", 96, 120};
    this->meat_profile_data[7] = {7, "Pork low", 74, 120};
    this->meat_profile_data[8] = {8, "Brisket", 91, 122};
    this->meat_profile_data[9] = {9, "Custom 1", -1, -1};
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

void View::drawMainPage(page_params_t &page_params)
{
    char buffer_t1[10];
    char buffer_t2[10];
    char buffer_t3[10];

    u8g2_ClearBuffer(&u8g2);
    u8g2_DrawBitmap(&u8g2, 0, 10, 10, 46, tank);

    u8g2_SetFont(&u8g2, u8g2_font_tinytim_tr);
    u8g2_DrawStr(&u8g2, 32, 7, "Smoker");

    // String for temperature tank
    u8g2_DrawStr(&u8g2, 28, 32, page_params.temp_tank1);
    u8g2_DrawStr(&u8g2, 50, 32, "C");

    u8g2_DrawLine(&u8g2, 26, 34, 56, 34);

    // Write the target temperature for the tank under the current temperature
    sprintf(buffer_t1, "%.1f", page_params.set_temp_tank);
    u8g2_DrawStr(&u8g2, 28, 42, buffer_t1);
    u8g2_DrawStr(&u8g2, 50, 42, "C");

    if(page_params.selected_option == 0) {

        u8g2_DrawBitmap(&u8g2, 82, 12, 1, 8, thermometer);
        u8g2_DrawBitmap(&u8g2, 82, 36, 1, 8, thermometer);
        u8g2_DrawBitmap(&u8g2, 82, 24, 1, 8, time_icon);
        u8g2_DrawBitmap(&u8g2, 82, 48, 1, 8, time_icon);

        if(index < 60) {
            // String for temperature 1
            u8g2_DrawStr(&u8g2, 92, 19, page_params.temp_meat1);
            // String for temperature 2
            u8g2_DrawStr(&u8g2, 92, 43, page_params.temp_meat2);
        }
        else if (index >= 80 && index < 120) {
            // String for time 1
            u8g2_DrawStr(&u8g2, 92, 19, page_params.set_temp_meat1);
            // String for time 2
            u8g2_DrawStr(&u8g2, 92, 43, page_params.set_temp_meat2);
        } else {
            index = 0;
        }

        // String for time 1
        u8g2_DrawStr(&u8g2, 92, 31, page_params.time_meat1);
        // String for time 2
        u8g2_DrawStr(&u8g2, 92, 55, page_params.time_meat2);
    }
    else {
        u8g2_DrawBitmap(&u8g2, 82, 12, 1, 8, thermometer);
        u8g2_DrawBitmap(&u8g2, 82, 24, 1, 8, thermometer);

        // String for bme temperature
        u8g2_DrawStr(&u8g2, 92, 19, page_params.bme280_data_temp);
        // String for bme humidity
        u8g2_DrawStr(&u8g2, 92, 31, page_params.bme280_data_hum);
    }

    index++;
    u8g2_SendBuffer(&u8g2);
}

void View::drawMenuPage(uint8_t selected_option) {
    u8g2_ClearBuffer(&u8g2);
    setPageHeader("Menu");


    if(selected_option < 1) {
        u8g2_DrawButtonUTF8(&u8g2, 8, 20, button_style[selected_option == 0], 0, 2, 2, "Meat profile");
        u8g2_DrawButtonUTF8(&u8g2, 8, 36, button_style[selected_option == 1], 0, 2, 2, "Settings");
    }
    
    else {
        u8g2_DrawButtonUTF8(&u8g2, 8, 20, button_style[selected_option == 1], 0, 2, 2, "Settings");
        u8g2_DrawButtonUTF8(&u8g2, 8, 36, button_style[selected_option == 2], 0, 2, 2, "Close fire");
    }

    u8g2_DrawButtonUTF8(&u8g2, 8, 60, button_style[selected_option == 3], 0, 2, 2, "Save");
    u8g2_DrawButtonUTF8(&u8g2, 90, 60, button_style[selected_option == 4], 0, 2, 2, "Exit");

    u8g2_SendBuffer(&u8g2);
}

void View::setPageHeader(const char *header) {
    // Center the text on the display and add * in the beginning and end of the header
    u8g2_DrawStr(&u8g2, 32, 7, header);
}

void View::drawMeatSelectionPage(uint8_t selected_option) {
    u8g2_ClearBuffer(&u8g2);
    setPageHeader("Meat profile");

    uint8_t index = (selected_option / 3) * 3;

    for (uint8_t i = 0; i < 3; i++) {
        u8g2_DrawButtonUTF8(&u8g2, 8, 20 + (i * 16), button_style[selected_option == (i + index)], 0, 2, 2, meat_profile_data[i + index].name);
        if(i + index > 9) {
            break;
        }
    }
    
    u8g2_DrawButtonUTF8(&u8g2, 90, 60, button_style[selected_option == 10], 0, 2, 2, "Back");

    u8g2_SendBuffer(&u8g2);
}

void View::drawSettingsPage(uint8_t selected_option) {
    u8g2_ClearBuffer(&u8g2);
    setPageHeader("Settings");

    // Setting for the temperature unit
    u8g2_DrawButtonUTF8(&u8g2, 8, 20, button_style[selected_option == 0], 0, 2, 2, "Temperature unit");

    // Purge the pellet burner
    u8g2_DrawButtonUTF8(&u8g2, 8, 36, button_style[selected_option == 1], 0, 2, 2, "Purge burner");

    u8g2_DrawButtonUTF8(&u8g2, 8, 60, button_style[selected_option == 2], 0, 2, 2, "Save");
    u8g2_DrawButtonUTF8(&u8g2, 90, 60, button_style[selected_option == 3], 0, 2, 2, "Exit");

    u8g2_SendBuffer(&u8g2);
}

void View::drawStandbyPage(uint8_t selected_option) {
    u8g2_ClearBuffer(&u8g2);

    u8g2_SendBuffer(&u8g2);
}