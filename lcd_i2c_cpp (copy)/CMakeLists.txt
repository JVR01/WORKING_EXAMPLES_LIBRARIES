cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(lcd_cpp_lib)

pico_sdk_init()

#add_subdirectory(lib)   #to work this command -> lib must have a Cmakefile
include_directories(./lib)

add_executable(lcd_i2c_cpp	
	#lcd_i2c_test.cpp
	lcd_i2c_quotes.cpp
        lib/lcd_i2c.cpp        
        )

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(lcd_i2c_cpp  pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_enable_stdio_usb(lcd_i2c_cpp 1)
pico_enable_stdio_uart(lcd_i2c_cpp 1)
pico_add_extra_outputs(lcd_i2c_cpp)

