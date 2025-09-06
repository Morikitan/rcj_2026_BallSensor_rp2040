#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ball/ball.hpp"
#include "communication/communication.hpp"
#include "pico/multicore.h"
#include "pico/sync.h"

void core1_entry();

spin_lock_t *lock;
volatile uint16_t pulse[16];

int main()
{
    stdio_init_all();
    //コア間で共有する変数の保護のロックを取得
    lock = spin_lock_instance(0); 
    //core1を始動
    multicore_launch_core1(core1_entry);

    I2CSetup();
    while(1){
        
    }
}

void core1_entry(){
    BallSetup();
    while(1){
        UseBallSensor();
    }
}
