# Smoker-UI

# Contents
- [Smoker-UI](#smoker-ui)
- [Contents](#contents)
- [Description](#description)
- [Installation](#installation)
- [Configuration](#configuration)
- [Planning](#planning)

# Description
This is a simple UI for the Smoker project. It is a application that runs on a ESP32-S3 wiht a ST7920 screen. The UI monitors the temperature of the smoker and the temperature of the meat. It also allows the user to set the temperature of the smoker and the meat. The UI also has a timer that can be set to monitor the time the meat has been cooking. The application also include a BLE server that can be used to control the smoker from a mobile device.

# Installation
The ESP32 program is build using ESP-IDF. To build the program you need to have the ESP-IDF installed. You can go to the [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) page to get the installation instructions. All the dependencies are included in the `CMakeLists.txt` file. The only thing that you need to do is to initialize the submodules. You can do this by running the following command in the root of the project.

```
git submodule update --init --recursive
```

The submodule is the library to control the ST7920 screen. The library is a fork of the original library that can be found. [here](https://github.com/olikraus/u8g2)

# Configuration
The configuration for the system is keep at a high layer. The configuration is done in the `Config.h` file. The file is located in the `config` folder. The file is a simple header file that contains the configuration for the system. The configuration is done using the `#define` directive. The configuration is divided into two categories, the configuration for the hardware and the configuration for the software. The configuration for the hardware is the configuration for the pins that are used to connect the screen and the temperature sensors. The configuration for the software is the configuration for the BLE server and the cooking profiles.

# Planning
Here is a simple descript ion of all the features that the UI will have. The features are divided into two categories, the features that are already implemented and the features that are still in development. The features are show on the first level of the list. The specific requirements of the features are shown on the second level of the list. The features that are already implemented are marked with a check mark. The features that are still in development are marked with a box.

On the ESP32:
- [ ] Monitor the temperature of the smoker
  - [ ] Set the temperature of the smoker to reach and maintain 
- [ ] Monitor the temperature of the meat
    - [ ] Monitor at least two temperatures
    - [ ] Set the temperature of the meat to reach
- [ ] Show the remaining time of the cooking
- [ ] Have a BLE server to control the smoker from a mobile device
  - [ ] Have a attribute to monitor the smoker temperature
  - [ ] Have a attribute to monitor the meat temperature
  - [ ] Have a attribute to monitor the remaining time of the cooking 
- [ ] Have pre-defined cooking profiles
  - [x] Pork profile
  - [x] Beef profile
  - [x] Chicken profile
  - [ ] Bacon profile
  - [x] Cold smoke profile
  - [x] General cooking profile 
  - [ ] Custom page profile
- [ ] Can control the smoker heating element
- [ ] Can control the smoker fan
  - [ ] Control the speed of the fan 
- [ ] Can control the smoker smoke generator
  - [ ] Control on and off 
- [ ] Monitor the environment temperature
- [ ] Monitor the environment humidity
- [ ] Monitor the environment pressure
- [ ] Have a simple UI to control the smoker
  - [x] Have a logo page
  - [ ] Have a icon to monitor the smoker temperature
  - [x] Have a icon to monitor the meat temperature (at least two temperatures)
  - [x] Have a menu to select the cooking profile
  - [ ] Have a icon to show the remaining time of the cooking
  - [ ] Have a page to control the smoker heating element
  - [ ] Have a page to control the smoker fan
  - [ ] Have a page to control the smoker smoke generator
  - [ ] Have a standby page
  
On the mobile device:
- [ ] Have a simple UI to control the smoker from a mobile device
  - [ ] Have meat temperature monitor
  - [ ] Have smoker temperature monitor
  - [ ] Have a timer
  - [ ] Have a menu to select the cooking profile
  - [ ] Have a page to control the smoker smoke generator
  - [ ] Have a set meat temperature
  - [ ] Have a set smoker temperature 
