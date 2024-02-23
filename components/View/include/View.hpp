#ifndef VIEW_HPP
#define VIEW_HPP

#include <array>
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include "MeatProfileData.h"
#include "pageDrawing.h"
#include "u8g2.h"
#include <u8g2_esp32_hal.hpp>
#include <string>

#define PIN_CLK GPIO_NUM_36
#define PIN_MOSI GPIO_NUM_40
#define PIN_RESET GPIO_NUM_38
#define PIN_CS GPIO_NUM_39

class View
{
private:
    u8g2_t u8g2;
    uint8_t index = 0;

    uint8_t button_style[2] = {U8G2_BTN_BW1, U8G2_BTN_INV};

    std::array<MeatProfileData, 10> meat_profile_data;

    void setPageHeader(const char *header);
public:
    View(/* args */);
    ~View();
    void initDisplay();
    void drawLogoPage();
    void drawMainPage(float temp_tank1, float temp_meat1, float temp_meat2,
                        float set_temp_tank, float set_temp_meat1, float set_temp_meat2,
                        uint32_t time_meat1, uint32_t time_meat2);
    /**
     * @brief The method draws the menu page
     * The page includes the following options:
     * 1. Probe 1 on/off
     * 2. Probe 2 on/off
     * 3. Profiles for cooking
     */
    void drawMenuPage(uint8_t selected_option);
    void drawMeatSelectionPage(uint8_t selected_option);
    //void drawSettingsPage(uint8_t selected_option);
};

#endif // VIEW_HPP