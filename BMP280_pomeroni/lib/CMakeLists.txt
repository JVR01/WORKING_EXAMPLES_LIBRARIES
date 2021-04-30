#add_library(lib bme280.cpp)


aux_source_directory(. DIR_bme280_SRCS)

add_library(lib ${DIR_bme280_SRCS})

target_link_libraries(lib PUBLIC pico_stdlib hardware_i2c)
