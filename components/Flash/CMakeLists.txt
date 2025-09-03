idf_component_register(SRCS "Flash.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES nvs_flash esp_partition log)
