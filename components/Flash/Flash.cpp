#include "Flash.hpp"

namespace Flash {
    bool init() {
        esp_err_t err = nvs_flash_init_partition("persistent");
        if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
            ESP_ERROR_CHECK(nvs_flash_erase_partition("persistent"));
            err = nvs_flash_init_partition("persistent");
        }
        if (err != ESP_OK) {
            ESP_LOGE("Flash", "nvs_flash_init_partition('persistent') failed: %s", esp_err_to_name(err));
        }
        return err == ESP_OK;
    }

    bool writeU16(const char* key, const flash_data_u16_t &data) {
        flash_data_u16_t write_data = data; // Create a copy to ensure correct size
        bool res = writeNVS(key, write_data.raw, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool writeU32(const char* key, const flash_data_u32_t &data) {
        flash_data_u32_t write_data = data; // Create a copy to ensure correct size
        bool res = writeNVS(key, write_data.raw, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool writeFloat(const char* key, const flash_data_float_t &data) {
        flash_data_float_t write_data = data; // Create a copy to ensure correct size
        bool res = writeNVS(key, write_data.raw, sizeof(data), NVS_TYPE_BLOB);
        return res;
    }

    bool readU16(const char* key, flash_data_u16_t &data) {
        flash_data_u16_t read_data;
        bool res = readNVS(key, read_data.raw, sizeof(data), NVS_TYPE_BLOB);
        data = read_data;
        return res;
    }

    bool readU32(const char* key, flash_data_u32_t &data) {
        flash_data_u32_t read_data = data;
        bool res = readNVS(key, read_data.raw, sizeof(data), NVS_TYPE_BLOB);
        data = read_data;
        return res;
    }

    bool readFloat(const char* key, flash_data_float_t &data) {
        flash_data_float_t read_data = data;
        bool res = readNVS(key, read_data.raw, sizeof(data), NVS_TYPE_BLOB);
        data = read_data;
        return res;
    }

    // Private
    namespace {
        static constexpr const char* kNvsPartition = "persistent";  // label in partitions.csv
        static constexpr const char* kNvsNamespace = "app";         // logical namespace inside the partition

        bool readNVS(const char* key, uint8_t *out_value, size_t length, nvs_type_t type) {
            nvs_handle_t h;
            esp_err_t err = nvs_open_from_partition(kNvsPartition, kNvsNamespace, NVS_READONLY, &h);
            if (err != ESP_OK) {
                ESP_LOGE("Flash", "nvs_open_from_partition('%s','%s', RO) failed: %s", kNvsPartition, kNvsNamespace, esp_err_to_name(err));
                return false;
            }

            err = nvs_get_blob(h, key, out_value, &length);
            if (err != ESP_OK) {
                ESP_LOGE("Flash", "nvs_get_blob('%s') failed: %s", key, esp_err_to_name(err));
            }
            nvs_close(h);
            return err == ESP_OK;
        }

        bool writeNVS(const char* key, const uint8_t *value, size_t length, nvs_type_t type) {
            nvs_handle_t h;
            esp_err_t err = nvs_open_from_partition(kNvsPartition, kNvsNamespace, NVS_READWRITE, &h);
            if (err != ESP_OK) {
                ESP_LOGE("Flash", "nvs_open_from_partition('%s','%s', RW) failed: %s", kNvsPartition, kNvsNamespace, esp_err_to_name(err));
                return false;
            }

            // Debug dump (optional)
            for (size_t i = 0; i < length; i++) {
                printf("Byte %zu: 0x%02X\n", i, ((const uint8_t*)value)[i]);
            }
            err = nvs_set_blob(h, key, value, length);
            if (err == ESP_OK) err = nvs_commit(h);
            if (err != ESP_OK) {
                ESP_LOGE("Flash", "nvs_set_blob/nvs_commit('%s') failed: %s", key, esp_err_to_name(err));
            }
            nvs_close(h);
            return err == ESP_OK;
        }
    }
}