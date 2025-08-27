add_library(communication communication.cpp)

target_link_libraries(communication pico_stdlib hardware_i2c)