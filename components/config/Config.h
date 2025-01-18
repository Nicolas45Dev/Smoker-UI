#ifndef CONFIG_H
#define CONFIG_H

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

#endif // CONFIG_H