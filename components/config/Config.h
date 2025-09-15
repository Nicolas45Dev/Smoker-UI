#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

typedef enum {
    CELSIUS_UNIT,
    FAHRENHEIT_UNIT,
    KELVIN_UNIT
} TEMP_UNIT;

typedef enum {
    PSI_UNIT,
    BAR_UNIT,
    HPA_UNIT,
    ATM_UNIT
} PERSSURE_UNIT;

#define DEFAULT_UNIT CELSIUS_UNIT
#define DEFAULT_PRESSURE_UNIT HPA_UNIT
#define TANK_SIZE 40
#define EPSILON 0.0001

// define a macro for temperature conversion
#define CELSIUS_TO_FAHRENHEIT(x) (x * 9 / 5 + 32)
#define CELSIUS_TO_KELVIN(x) (x + 273.15)
#define FAHRENHEIT_TO_CELSIUS(x) ((x - 32) * 5 / 9)
#define FAHRENHEIT_TO_KELVIN(x) ((x - 32) * 5 / 9 + 273.15)
#define KELVIN_TO_CELSIUS(x) (x - 273.15)
#define KELVIN_TO_FAHRENHEIT(x) ((x - 273.15) * 9 / 5 + 32)
#define PASCAL_TO_HECTOPASCAL(x) (x / 100)
#define PASCAL_TO_ATMOSPHERE(x) (x / 101325)
#define PASCAL_TO_BAR(x) (x / 100000)
#define PASCAL_TO_PSI(x) (x / 6894.75729)

#define ADC_MAX_VALUE 4095.0
#define ADC_REF_VOLTAGE 3.3

// ADC channels
#define T1_TEMP ADC_CHANNEL_3 // GPIO36
#define T2_TEMP ADC_CHANNEL_4 // GPIO39
#define TINT_TEMP ADC_CHANNEL_5 // GPIO34

// GPIO for the MCP23017 IO expander
namespace MCP23017Pins {
    const uint8_t RELAY1_PIN = 43;
    const uint8_t RELAY2_PIN = 44;
    const uint8_t PROBE_DETECT1_PIN = 45;
    const uint8_t PROBE_DETECT2_PIN = 46;
    const uint8_t PROBE_DETECT3_PIN = 47;
    const uint8_t BUZZER_PIN = 41;
    const uint8_t EN5V_PIN = 40;
    const uint8_t EN_MOTOR_PIN = 57;
}

// GPIO for the esp32
namespace ESP32Pins {
    const uint8_t ENCODER_A_PIN = 32;
    const uint8_t MOTOR_PWM_PIN = 10;
    const uint8_t TACHO_FAN_PIN = 8;
    const uint8_t FAN_PWM_PIN = 7;
}

#endif // CONFIG_H