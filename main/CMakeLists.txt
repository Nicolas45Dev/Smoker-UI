idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES console vfs driver esp_partition log Controller GPIO Model Fan config)
