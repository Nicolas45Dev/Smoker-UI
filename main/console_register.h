#ifndef CONSOLE_REGISTER_H
#define CONSOLE_REGISTER_H

#include "sdkconfig.h"
#include "Config.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "esp_log.h"
#include <inttypes.h>
#include <GPIO.hpp>

#define CLI_TAG "console"

const char *SET_LED_CMD = "set_led";
const char *GET_TEMP_CMD = "temp";
const char *SET_FAN_CMD = "set_fan";
const char *SET_MOTOR_CMD = "set_motor";
const char *SET_RELAY_CMD = "set_relay";
const char *ADC_RESET_CMD = "adc_reset";
const char *ADC_CALIB_CMD = "adc_calib";

// Turn the value check into a type-safe inline function (avoids macro pitfalls)
static inline int cli_check_value_limits(int min, int max, int val) {
    if ((val) < (min) || (val) > (max)) {
        ESP_LOGE(CLI_TAG, "Invalid argument, value=%d, expected range: %d..%d", (int)val, (int)min, (int)max);
        return 1;
    }
    return 0;
}
// Backward-compatible macro wrapper
#define CLI_CHECK_VALUE_LIMITS(min, max, val) cli_check_value_limits((int)(min), (int)(max), (int)(val))

#ifdef CONFIG_APP_CONSOLE_ENABLED

/**
 * @brief Get the temperature reading from a specified sensor
 * 
 * @param argc Argument count
 * @param argv Argument values, Should contain sensor name
 * @return int 0 on success, non-zero on failure
 */
static int cmd_get_temp(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: temp <sensor_name>\n");
        return 1;
    }
    const char *sensor_name = argv[1];
    ESP_LOGI("temp", "Getting temperature from sensor '%s'", sensor_name);
    // Simulate a temperature reading
    int32_t temperature = 2500; // Dummy value as milli-degrees Celsius
    printf("Temperature from '%s': %" PRId32 "\n", sensor_name, temperature);
    return 0;
}

/**
 * @brief Set the fan speed using the name and speed value
 * 
 * @param argc Argument count
 * @param argv Argument values, Should contain speed and name
 * @return int 0 on success, non-zero on failure
 */
static int cmd_set_fan(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: set_fan <speed>\n");
        return 1;
    }
    int speed = atoi(argv[1]);
    uint8_t res = CLI_CHECK_VALUE_LIMITS(0, 100, speed);
    if (res) {
        return res;
    }
    ESP_LOGI("set_fan", "Setting fan speed to %d%%", speed);
    return 0;
}

/**
 * @brief Set the relay state using the relay number and state value
 * 
 * @param argc Argument count
 * @param argv Argument values, Should contain relay number and state
 * @return int 0 on success, non-zero on failure
 */
static int cmd_set_relay(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: set_relay <relay_number> <on|off>\n");
        return 1;
    }
    int relay_number = atoi(argv[1]);
    const char *state = argv[2];
    uint8_t res = CLI_CHECK_VALUE_LIMITS(0, 2, relay_number); // Assuming max 2 relays
    if (res) {
        return res;
    }
    if (strcmp(state, "on") != 0 && strcmp(state, "off") != 0) {
        printf("Invalid state. Use 'on' or 'off'.\n");
        return 1;
    }
    ESP_LOGI("set_relay", "Setting relay %d to state '%s'", relay_number, state);
    GPIO::digitalWrite((MCP23017Pins::RELAY1_PIN + relay_number), strcmp(state, "on") == 0 ? 1 : 0);
    return 0;
}

/**
 * @brief Set the motor speed using the name and speed value
 * 
 * @param argc Argument count
 * @param argv Argument values, Should contain speed and name
 * @return int 0 on success, non-zero on failure
 */
static int cmd_set_motor(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: set_motor <speed>\n");
        return 1;
    }
    int speed = atoi(argv[1]);
    uint8_t res = CLI_CHECK_VALUE_LIMITS(0, 100, speed);
    if (res) {
        return res;
    }
    ESP_LOGI("set_motor", "Setting motor speed to %d%%", speed);
    return 0;
}

/**
 * @brief Reset the ADC calibration data for all channels
 * 
 * @param argc Argument count
 * @param argv Argument values
 * @return int 0 on success, non-zero on failure
 */
static int cmd_adc_reset(int argc, char **argv)
{
    ESP_LOGI("adc_reset", "Resetting ADC calibration data");
    // Implement ADC reset logic here
    return 0;
}

/**
 * @brief Calibrate the ADC with a given calibration value
 * 
 * @param argc Argument count
 * @param argv Argument values, Should contain calibration value
 * @return int 0 on success, non-zero on failure
 */
static int cmd_adc_calib(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: adc_calib <calibration_value>\n");
        return 1;
    }
    int calib_value = atoi(argv[1]);
    ESP_LOGI("adc_calib", "Calibrating ADC with value %d", calib_value);
    // Implement ADC calibration logic here
    return 0;
}

static void register_commands()
{
    const esp_console_cmd_t temperature_cmd = {
        .command = "temp",
        .help = "Get temperature readings <sensor_name>",
        .hint = NULL,
        .func = &cmd_get_temp,
        .argtable = NULL,
    };

    const esp_console_cmd_t fan_cmd = {
        .command = "set_fan",
        .help = "Set fan speed. Usage: set_fan <speed 0-100>",
        .hint = NULL,
        .func = &cmd_set_fan,
        .argtable = NULL,
    };

    const esp_console_cmd_t motor_cmd = {
        .command = "set_motor",
        .help = "Set motor speed. Usage: set_motor <speed 0-100>",
        .hint = NULL,
        .func = &cmd_set_motor,
        .argtable = NULL,
    };

    const esp_console_cmd_t relay_cmd = {
        .command = "set_relay",
        .help = "Set relay state. Usage: set_relay <relay_number> <on|off>",
        .hint = NULL,
        .func = &cmd_set_relay,
        .argtable = NULL,
    };

    const esp_console_cmd_t adc_reset_cmd = {
        .command = "adc_reset",
        .help = "Reset ADC calibration data",
        .hint = NULL,
        .func = &cmd_adc_reset,
        .argtable = NULL,
    };

    const esp_console_cmd_t adc_calib_cmd = {
        .command = "adc_calib",
        .help = "Calibrate ADC. Usage: adc_calib <calibration_value>",
        .hint = NULL,
        .func = &cmd_adc_calib,
        .argtable = NULL,
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&temperature_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&fan_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&motor_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&relay_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&adc_reset_cmd));
    ESP_ERROR_CHECK(esp_console_cmd_register(&adc_calib_cmd));
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

    // Line endings (use non-deprecated, port-specific APIs)
    esp_vfs_dev_uart_port_set_rx_line_endings(UART_NUM_0, ESP_LINE_ENDINGS_CR);
    esp_vfs_dev_uart_port_set_tx_line_endings(UART_NUM_0, ESP_LINE_ENDINGS_CRLF);

    const uart_config_t uart_config = {
        .baud_rate = CONFIG_APP_CONSOLE_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
    #if SOC_UART_SUPPORT_REF_TICK
        .source_clk = UART_SCLK_REF_TICK,
    #else
        .source_clk = UART_SCLK_DEFAULT,
    #endif
        .flags = 0,
    };
    // ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));
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