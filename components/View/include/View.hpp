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

#define PIN_CLK (gpio_num_t)40
#define PIN_MOSI (gpio_num_t)41
#define PIN_RESET (gpio_num_t)45
#define PIN_CS (gpio_num_t)42

typedef struct {
    char* temp_tank1;
    char* temp_meat1;
    char* temp_meat2;
    float set_temp_tank;
    float set_temp_meat1;
    float set_temp_meat2;
    uint32_t time_meat1;
    uint32_t time_meat2;
    float* bme280_data;
    uint8_t selected_option;
} page_params_t;

class View
{
private:
    u8g2_t u8g2;
    uint8_t index = 0;

    uint8_t button_style[2] = {U8G2_BTN_BW1, U8G2_BTN_INV};

    std::array<MeatProfileData, 10> meat_profile_data;

    // Set the meat profile in alphabetical order
    meat_profile_data[0] = {0, "Bacon", 65, 120};
    meat_profile_data[1] = {1, "Beef", 55, 90};
    meat_profile_data[2] = {2, "Chicken", 84, 120};
    meat_profile_data[3] = {3, "Cold Smoke", -1, -1};
    meat_profile_data[4] = {4, "Fish", 60, 120};
    meat_profile_data[5] = {5, "General smoking", -1, 120};
    meat_profile_data[6] = {6, "Pork high", 96, 120};
    meat_profile_data[7] = {7, "Pork low", 74, 120};
    meat_profile_data[8] = {8, "Custom 1", -1, -1};
    meat_profile_data[9] = {9, "Custom 2", -1, -1};

    void setPageHeader(const char *header);
public:
    View(/* args */);
    ~View();
    void initDisplay();
    void drawLogoPage();

    /**
     * @brief Show the main page
     * 
     * @param temp_tank1 the temperature of the tank
     * @param temp_meat1 the temperature of the first meat probe
     * @param temp_meat2 the temperature of the second meat probe
     * @param set_temp_tank the target temperature of the tank
     * @param set_temp_meat1 the target temperature of the first meat probe
     * @param set_temp_meat2 the target temperature of the second meat probe
     * @param time_meat1 the estimated remining time for the first meat probe
     * @param time_meat2 the estimated remining time for the second meat probe
     * @param bme280_data the data from the BME280 sensor this includes the temperature, humidity and pressure
     */
    void drawMainPage(page_params_t &page_params);
    /**
     * @brief The method draws the menu page
     * The page includes the following options:
     * 1. Probe 1 on/off
     * 2. Probe 2 on/off
     * 3. Profiles for cooking
     */
    void drawMenuPage(uint8_t selected_option);

    /**
     * @brief Show the meat selection page
     * 
     * @param selected_option 
     */
    void drawMeatSelectionPage(uint8_t selected_option);

    /**
     * @brief Show the settings page
     * 
     * @param selected_option 
     */
    void drawSettingsPage(uint8_t selected_option);
};

#endif // VIEW_HPP