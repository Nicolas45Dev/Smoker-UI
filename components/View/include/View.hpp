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
    uint8_t selected_option;

    char* bme280_data_hum;
    char* bme280_data_temp;
    char* set_temp_meat1;
    char* set_temp_meat2;
    char* temp_meat1;
    char* temp_meat2;
    char* temp_tank1;

    uint32_t time_meat1;
    uint32_t time_meat2;

    float set_temp_tank;
} page_params_t;

class View
{
private:
    u8g2_t u8g2{};
    uint8_t index = 0;

    uint8_t button_style[2] = {U8G2_BTN_BW1, U8G2_BTN_INV};

    /**
     * @brief Set the header of a page with the desired text
     * @param header The text to be displayed as the header
     */
    void setPageHeader(const char *header);

    /**
     * @brief This function formats the time to a string from a given time in seconds to a time format HH:MM
     * @param time The time in seconds
     * @param buffer The buffer to store the formatted time
     */
    void formatTime(const uint32_t time, char* buffer);
public:
    View(/* args */);
    ~View();
    void initDisplay();
    void drawLogoPage();

    /**
     * @brief Show the main page
     * 
     * @param page_params Struct for the page options
     */
    void drawMainPage(const page_params_t &page_params);

    /**
    * @brief Show the meat selection page
    *
    * @param selected_option The selected option
    * @param profile The array of meat profiles
    */
    void drawMeatSelectionPage(const uint8_t selected_option, const std::array<MeatProfileData, 10> &profile);

    /**
     * @brief The method draws the menu page
     * The page includes the following options:
     * 1. Meat profiles
     * 2. Settings
     * 3. Controls
     *
     * @param selected_option The selected option
     */
    void drawMenuPage(const uint8_t selected_option);

    /**
     * @brief Show the settings page
     * 
     * @param selected_option The selected option
     */
    void drawSettingsPage(const uint8_t selected_option);

    /**
     * @brief Show the save page
     * 
     * @param selected_option The selected option
     */
    void drawStandbyPage(const uint8_t selected_option);

    /**
     * @brief Show the temperature settings page
     * 
     * @param selected_option The selected option
     */
    void drawTemperatureSettingsPage(const uint8_t selected_option);
};

#endif // VIEW_HPP