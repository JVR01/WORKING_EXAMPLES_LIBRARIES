cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(W_Station_project)
pico_sdk_init()

add_subdirectory(ds1307)
include_directories(./ds1307)

add_executable(w_station
        w_station.c
        )

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(w_station ds1307 pico_stdlib hardware_i2c hardware_spi)


pico_enable_stdio_usb(w_station 1)
pico_enable_stdio_uart(w_station 1)
# create map/bin/hex file etc.
pico_add_extra_outputs(w_station)

# add url via pico_set_program_url
#example_auto_set_url(lcd_1602_i2c)

############################################

