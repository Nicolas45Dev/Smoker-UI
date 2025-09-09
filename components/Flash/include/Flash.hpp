#ifndef FLASH_HPP
#define FLASH_HPP

#include <stdint.h>
#include <string>
#include "nvs_flash.h"
#include "esp_partition.h"
#include "esp_log.h"


// Helper struct types to compute sizes reliably (avoid relying on padding guesses)
typedef struct flash_data_u16_struct_t {
    uint32_t version;
    uint16_t value;
    uint32_t crc;
} flash_data_u16_struct_t;

typedef union {
    struct {
        uint32_t version;
        uint16_t value;
        uint32_t crc;
    };
    uint8_t raw[sizeof(flash_data_u16_struct_t)];
} flash_data_u16_t;

typedef struct flash_data_u32_struct_t {
    uint32_t version;
    uint32_t value;
    uint32_t crc;
} flash_data_u32_struct_t;

typedef union {
    struct {
        uint32_t version;
        uint32_t value;
        uint32_t crc;
    };
    uint8_t raw[sizeof(flash_data_u32_struct_t)];
} flash_data_u32_t;

typedef struct flash_data_float_struct_t {
    uint32_t version;
    float value;
    uint32_t crc;
} flash_data_float_struct_t;

typedef union {
    struct {
        uint32_t version;
        float value;
        uint32_t crc;
    };
    uint8_t raw[sizeof(flash_data_float_struct_t)];
} flash_data_float_t;

// Sanity checks: ensure union size matches the struct size we expect
static_assert(sizeof(flash_data_u16_t) == sizeof(flash_data_u16_struct_t), "flash_data_u16_t size mismatch");
static_assert(sizeof(flash_data_u32_t) == sizeof(flash_data_u32_struct_t), "flash_data_u32_t size mismatch");
static_assert(sizeof(flash_data_float_t) == sizeof(flash_data_float_struct_t), "flash_data_float_t size mismatch");

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
        bool readNVS(const char* key, uint8_t *out_value, size_t length, nvs_type_t type);
        bool writeNVS(const char* key, const uint8_t *value, size_t length, nvs_type_t type);
    }
}

#endif // FLASH_HPP