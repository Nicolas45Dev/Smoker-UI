idf_component_register(SRCS "WUI.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_server nvs_flash Flash esp_wifi)
