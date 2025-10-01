/**
 * @file WUI.hpp
 * @author Nicolas Biard (nico645@ouook.com)
 * @brief 
 * @version 0.1
 * @date 28-09-2025
 * The file WUI represents the Web User Interface component of the project.
 * The file encapsulates all the files and functionalities related to the web interface.
 * It reads from the flash to check if the ESP32 needs to hotpot mode or not.
 * 
 */
#ifndef WUI_HPP
#define WUI_HPP

// Include the configuration file from the Kconfig
#include "sdkconfig.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_http_server.h>
#include <Flash.hpp>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define AP_SSID "Smoker-UI"
#define AP_PASSWORD "smokerui123"

namespace WUI {
    bool init();
    bool isConnected();
}

#endif