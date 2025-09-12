idf_component_register(SRCS "esp32_hal_spi.cpp" "SPI.cpp" "BME280.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver config)
