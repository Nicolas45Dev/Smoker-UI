#ifndef MEAT_PROFILE_DATA_H
#define MEAT_PROFILE_DATA_H

#include <stdint.h>

/**
 * @brief A struct to hold the meat profile data
 * 
 * @param id The id of the meat profile
 * @param name The name of the meat profile in a char array (max 20 characters)
 * @param meat_temp The temperature of the meat to reach (in Celsius)
 * @param tank_temp The temperature of the tank (in Celsius)
 */
typedef struct {
    uint8_t id;
    char name[20];
    float meat_temp;
    float tank_temp;
} MeatProfileData;

#endif // MEAT_PROFILE_DATA_H