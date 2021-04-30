aux_source_directory(. DIR_ds1307_SRCS)

add_library(ds1307 ${DIR_ds1307_SRCS})
target_link_libraries(ds1307 PUBLIC pico_stdlib hardware_i2c)
