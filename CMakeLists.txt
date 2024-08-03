
cmake_minimum_required(VERSION 3.13)

# Environmental variable DIR constants that must be defined:
# PICO_SDK_PATH
# PICO_LCD_PATH 
# ZLIB_RP2XX_PATH

# DIR constants.
# Initialize SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)

project(Picovideo VERSION 1.0)

# Initialize RPi Pico SDK
pico_sdk_init()

# Include root directory to make data and framerate files available
include_directories(./)

include($ENV{ZLIB_RP2XX_PATH}/interfaceLibForPicoSDK.cmake)

# Include dirs to Pico LCD libraries SOURCE
set(PICO_LCD_CODE $ENV{PICO_LCD_PATH}/c)
include_directories(${PICO_LCD_CODE}/lib/Config)
include_directories(${PICO_LCD_CODE}/lib/GUI)
include_directories(${PICO_LCD_CODE}/lib/Fonts)
include_directories(${PICO_LCD_CODE}/lib/LCD)

add_subdirectory(${PICO_LCD_CODE}/lib/Config Pico_LCD/Config)
add_subdirectory(${PICO_LCD_CODE}/lib/LCD Pico_LCD/LCD)
add_subdirectory(${PICO_LCD_CODE}/lib/Fonts Pico_LCD/Fonts)
add_subdirectory(${PICO_LCD_CODE}/lib/GUI Pico_LCD/GUI)

# Create screens library
include_directories(./screens)
add_subdirectory(screens)


add_executable(video
  video.c
)

target_link_libraries(video screens Config GUI Fonts LCD pico_stdlib hardware_spi zlib)

# Create map/bin/hex/uf2 file in addition to ELP
pico_add_extra_outputs(video)
