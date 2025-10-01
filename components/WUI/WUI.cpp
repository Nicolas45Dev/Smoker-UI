#include "WUI.hpp"

static const char* TAG = "WUI";

bool WUI::init() {
   flash_data_u16_t wifi_mode;
   Flash::readU16("wifi_mode", wifi_mode);
   uint16_t mode = wifi_mode.value;

   // If wifi_mode is 0, start in AP mode (Hotspot) else try to connect to the saved network
   if (mode == 0) {
        ESP_LOGI(TAG, "Starting in AP mode");
   } else if (mode == 1) {
        ESP_LOGI(TAG, "Starting in STA mode");
   }
   
    return true;
}

bool WUI::isConnected() {
    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        return true;
    } else {
        return false;
    }
}