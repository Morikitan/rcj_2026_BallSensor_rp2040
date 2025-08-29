add_library(communication communication.cpp)

add_subdirectory(pico_i2c_slave-master)

target_include_directories(communication PRIVATE pico_i2c_slave-master)

target_link_libraries(communication pico_stdlib hardware_i2c hardware_gpio)
target_link_libraries(communication i2c_slave)