
cmake_minimum_required(VERSION 3.13)

# Environmental variable DIR constants that must be defined:
# PICO_SDK_PATH
# PICO_LCD_PATH 

# DIR constants.
# Initialize SDK based on PICO_SDK_PATH
include(../pico_sdk_import.cmake)

project(ram_find VERSION 1.0)

# Initialize RPi Pico SDK
pico_sdk_init()

# Include root directory to make data and framerate files available
include_directories(./)

set(PICO_LCD_CODE $ENV{PICO_LCD_PATH}/c)

# Include dirs to Pico LCD libraries SOURCE
include_directories(${PICO_LCD_CODE}/lib/Config)
include_directories(${PICO_LCD_CODE}/lib/GUI)
include_directories(${PICO_LCD_CODE}/lib/Fonts)
include_directories(${PICO_LCD_CODE}/lib/LCD)

add_subdirectory(${PICO_LCD_CODE}/lib/Config Pico_LCD/Config)
add_subdirectory(${PICO_LCD_CODE}/lib/LCD Pico_LCD/LCD)
add_subdirectory(${PICO_LCD_CODE}/lib/Fonts Pico_LCD/Fonts)
add_subdirectory(${PICO_LCD_CODE}/lib/GUI Pico_LCD/GUI)


add_executable(ram_find
  ram_find.c
)

target_link_libraries(ram_find Config GUI Fonts LCD pico_stdlib)

# Create map/bin/hex/uf2 file in addition to ELP
pico_add_extra_outputs(ram_find)
