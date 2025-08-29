#include "ball.hpp"
#include "../config.hpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"
#include "hardware/pio.h"

//ボールセンサーの初期化
void BallSetup(){
    gpio_init(Sensorpin0); gpio_init(Sensorpin1);
    gpio_init(Sensorpin2); gpio_init(Sensorpin3);
    gpio_init(Sensorpin4); gpio_init(Sensorpin5);
    gpio_init(Sensorpin6); gpio_init(Sensorpin7);
    gpio_init(Sensorpin8); gpio_init(Sensorpin9);
    gpio_init(Sensorpin10); gpio_init(Sensorpin11);
    gpio_init(Sensorpin12); gpio_init(Sensorpin13);
    gpio_init(Sensorpin14); gpio_init(Sensorpin15);
    gpio_set_dir(Sensorpin0,false); gpio_set_dir(Sensorpin1,false);
    gpio_set_dir(Sensorpin2,false); gpio_set_dir(Sensorpin3,false);
    gpio_set_dir(Sensorpin4,false); gpio_set_dir(Sensorpin5,false);
    gpio_set_dir(Sensorpin6,false); gpio_set_dir(Sensorpin7,false);
    gpio_set_dir(Sensorpin8,false); gpio_set_dir(Sensorpin9,false);
    gpio_set_dir(Sensorpin10,false); gpio_set_dir(Sensorpin11,false);
    gpio_set_dir(Sensorpin12,false); gpio_set_dir(Sensorpin13,false);
    gpio_set_dir(Sensorpin14,false); gpio_set_dir(Sensorpin15,false);
}

//ボールセンサー(赤外線センサー)を使う。
void UseBallSensor(){
    
}

