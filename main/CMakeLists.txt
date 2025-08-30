idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES console vfs driver Controller Encoder)

# If USB console is enabled, make sure tinyusb is available (safe to keep as overall project dep via upper CMake)
# You can add tinyusb to the top-level or relevant component if needed.
