#ifndef CONSOLE_REGISTER_H
#define CONSOLE_REGISTER_H

#include "sdkconfig.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "esp_log.h"

#define CLI_TAG "console"

#define CLI_CHECK_VALUE_LIMITS(min, max, val) do { \
    if ((val) < (min) || (val) > (max)) { \
        ESP_LOGE(CLI_TAG, "Invalid argument, value=%d, expected range: %d..%d", val, min, max); \
        return 1; \
    } \
} while(0)

#ifdef CONFIG_APP_CONSOLE_ENABLED
// Example command: set_led <intensity> <name>
static int cmd_set_led(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: set_led <intensity> <name>\n");
        return 1;
    }
    int intensity = atoi(argv[1]);
    const char *name = argv[2];
    uint8_t res = CLI_CHECK_VALUE_LIMITS(0, 100, intensity);
    if (res) {
        return res;
    }
    ESP_LOGI("set_led", "Setting LED '%s' to intensity %d", name, intensity);
    return 0;
}

static void register_commands()
{
    const esp_console_cmd_t cmd = {
        .command = "set_led",
        .help = "Set an LED intensity. Usage: set_led <intensity 0-99> <name>",
        .hint = NULL,
        .func = &cmd_set_led,
        .argtable = NULL,
    };
    ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

// Initialize interactive console according to Kconfig (UART or USB CDC)
static void init_console()
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_cfg = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    repl_cfg.prompt = "esp> ";
    repl_cfg.max_cmdline_length = 256;

#if CONFIG_APP_CONSOLE_UART
    // Disable buffering on stdio
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    // Line endings
    esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    const uart_config_t uart_config = {
        .baud_rate = CONFIG_APP_CONSOLE_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    #if SOC_UART_SUPPORT_REF_TICK
        .source_clk = UART_SCLK_REF_TICK,
    #else
        .source_clk = UART_SCLK_DEFAULT,
    #endif
    };
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
    esp_vfs_dev_uart_use_driver(UART_NUM_0);

    esp_console_dev_uart_config_t dev_cfg = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    dev_cfg.channel = UART_NUM_0;
    dev_cfg.tx_gpio_num = UART_PIN_NO_CHANGE;
    dev_cfg.rx_gpio_num = UART_PIN_NO_CHANGE;

    ESP_ERROR_CHECK(esp_console_register_help_command());
    register_commands();
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&dev_cfg, &repl_cfg, &repl));
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
#elif CONFIG_APP_CONSOLE_USB
    // USB CDC REPL (requires enabling TinyUSB CDC-ACM in menuconfig)
    ESP_ERROR_CHECK(esp_console_register_help_command());
    register_commands();

    esp_console_dev_usb_cdc_config_t usb_cfg = ESP_CONSOLE_DEV_USB_CDC_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_cdc(&usb_cfg, &repl_cfg, &repl));
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
#endif
}
#endif // CONFIG_APP_CONSOLE_ENABLED


#endif // CONSOLE_REGISTER_H