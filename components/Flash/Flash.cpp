#include "Flash.hpp"

namespace Flash {
    bool init() {
        esp_err_t err = nvs_flash_init_partition("persistent");
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase_partition("persistent"));
            err = nvs_flash_init_partition("persistent");
        }
        
        return err == ESP_OK;
    }

    bool writeU16(const char* key, const flash_data_u16_t &data) {
        bool res = writeNVS(key, &data, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool writeU32(const char* key, const flash_data_u32_t &data) {
        bool res = writeNVS(key, &data, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool writeFloat(const char* key, const flash_data_float_t &data) {
        bool res = writeNVS(key, &data, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool readU16(const char* key, flash_data_u16_t &data) {
        bool res = readNVS(key, &data, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool readU32(const char* key, flash_data_u32_t &data) {
        bool res = readNVS(key, &data, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool readFloat(const char* key, flash_data_float_t &data) {
        bool res = readNVS(key, &data, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    // Private
    namespace {
        bool readNVS(const char* key, void *out_value, size_t length, nvs_type_t type) {
            nvs_handle_t h;
            esp_err_t err = nvs_open("persistent", NVS_READONLY, &h);
            if (err != ESP_OK) return false;

            err = nvs_get_blob(h, key, out_value, &length);
            nvs_close(h);
            return err == ESP_OK;
        }

        bool writeNVS(const char* key, const void *value, size_t length, nvs_type_t type) {
            nvs_handle_t h;
            esp_err_t err = nvs_open("persistent", NVS_READWRITE, &h);
            if (err != ESP_OK) return false;

            err = nvs_set_blob(h, key, value, length);
            if (err == ESP_OK) err = nvs_commit(h);
            nvs_close(h);
            return err == ESP_OK;
        }
    }
}