add_library(ball ball.cpp)

target_link_libraries(ball pico_stdlib hardware_gpio hardware_pio)
target_link_libraries(ball pico_multicore pico_sync) 

#pulsewidth.pioを使えるようにする処理を入れる
pico_generate_pio_header(ball ${CMAKE_CURRENT_LIST_DIR}/pin_monitoring.pio)