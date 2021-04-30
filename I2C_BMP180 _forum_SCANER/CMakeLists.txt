cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(I2C_BMP180)
pico_sdk_init()

#add_subdirectory(ds1307)
#include_directories(./ds1307)

add_executable(I2C_BMP180
        test.c
        
        )

#bmp180.h

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(I2C_BMP180 pico_stdlib hardware_i2c hardware_spi)


pico_enable_stdio_usb(I2C_BMP180 1)
pico_enable_stdio_uart(I2C_BMP180 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(I2C_BMP180)

# add url via pico_set_program_url
#example_auto_set_url(lcd_1602_i2c)

############################################

