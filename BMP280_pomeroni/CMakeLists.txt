cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(pomeroni_temp)

pico_sdk_init()

add_subdirectory(lib)
include_directories(./lib)

add_executable(temp
        demo.cpp
        #bme280.cpp
        #bme280.hpp
        )


target_link_libraries(temp lib hardware_i2c pico_stdlib)
#target_link_libraries(temp hardware_i2c pico_stdlib)

pico_enable_stdio_usb(temp 1)
pico_enable_stdio_uart(temp 1)

pico_add_extra_outputs(temp)

# Pull in our pico_stdlib which pulls in commonly used features

