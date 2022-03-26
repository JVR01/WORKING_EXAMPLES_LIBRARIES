cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(Simple_Serial_Read)
pico_sdk_init()

add_executable(${PROJECT_NAME}
    serial_read.c
)

target_link_libraries(${PROJECT_NAME} 
  pico_stdlib 
  hardware_uart
)

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)




