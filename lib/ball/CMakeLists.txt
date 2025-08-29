add_library(ball ball.cpp)

target_link_libraries(ball pico_stdlib hardware_gpio hardware_pio)

#pulsewidth.pioを使えるようにする処理を入れる