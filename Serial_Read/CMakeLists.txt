cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(Serial_Read)
pico_sdk_init()

add_executable(serial_read
    serial_read.c
)

target_link_libraries(serial_read pico_stdlib hardware_uart)  #



pico_enable_stdio_usb(serial_read 1)
pico_enable_stdio_uart(serial_read 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(serial_read)


