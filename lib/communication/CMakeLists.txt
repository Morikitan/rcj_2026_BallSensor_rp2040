add_library(communication communication.cpp)

add_subdirectory(pico_i2c_slave-master)

target_include_directories(communication PRIVATE pico_i2c_slave-master)



pico_generate_pio_header(communication ${CMAKE_CURRENT_LIST_DIR}/picoPioUart.pio)

target_link_libraries(communication pico_stdlib hardware_i2c hardware_gpio hardware_pio)
target_link_libraries(communication i2c_slave pico_multicore pico_sync)