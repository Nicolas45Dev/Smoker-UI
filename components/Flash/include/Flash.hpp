#ifndef FLASH_HPP
#define FLASH_HPP

#include <stdint.h>
#include <string>
#include "nvs_flash.h"
#include "esp_partition.h"


typedef struct {
    uint32_t version;
    uint16_t value;
    uint32_t crc;
} flash_data_u16_t;

typedef struct {
    uint32_t version;
    uint32_t value;
    uint32_t crc;
} flash_data_u32_t;

typedef struct {
    uint32_t version;
    float value;
    uint32_t crc;
} flash_data_float_t;

namespace Flash {
    bool init();
    bool writeU16(const char* key, const flash_data_u16_t &data);
    bool writeU32(const char* key, const flash_data_u32_t &data);
    bool writeFloat(const char* key, const flash_data_float_t &data);
    bool readU16(const char* key, flash_data_u16_t &data);
    bool readU32(const char* key, flash_data_u32_t &data);
    bool readFloat(const char* key, flash_data_float_t &data);

    // Private
    namespace {
        bool readNVS(const char* key, void *out_value, size_t length, nvs_type_t type);
        bool writeNVS(const char* key, const void *value, size_t length, nvs_type_t type);
    }
}

#endif // FLASH_HPP