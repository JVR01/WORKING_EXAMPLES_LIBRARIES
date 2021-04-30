cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(WheaterEstation)

pico_sdk_init()

#add_subdirectory(lib)   #to work this command -> lib must have a Cmakefile
include_directories(./lib)


add_executable(WheaterEstation
        main.cpp	
	#lcd_i2c_test.cpp
	#lcd_i2c_quotes.cpp
	lib/bme280.cpp
        lib/lcd_i2c.cpp 
        lib/ds3231.c
        lib/ws2812.pio 
        lib/ws2812.c        
        )

#add this for the pixel library, it genertes the ws2812.pio.h understood by cpp
pico_generate_pio_header(WheaterEstation ${CMAKE_CURRENT_LIST_DIR}/lib/ws2812.pio)


#--target_link_libraries(WheaterEstation  pico_stdlib hardware_i2c)
target_link_libraries(WheaterEstation PRIVATE pico_stdlib hardware_i2c hardware_pio)

# create map/bin/hex file etc.
pico_enable_stdio_usb(WheaterEstation 1)
pico_enable_stdio_uart(WheaterEstation 1)
pico_add_extra_outputs(WheaterEstation)

