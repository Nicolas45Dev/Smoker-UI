idf_component_register(SRCS "MCP23017.cpp" "GPIO.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver Sensors config)
