set(CMAKE_CXX_STANDARD 17)
idf_component_register(SRCS "View.cpp" "u8g2_esp32_hal.cpp" "View.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES u8g2 driver)
