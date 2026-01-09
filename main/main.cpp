#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "ball/ball.hpp"
#include "communication/communication.hpp"
#include "config.hpp"

volatile uint16_t pulse[16];

int main()
{
    stdio_init_all();
    BallSetup();

    I2CSetup();
    
    while(1){
        int preTime = timer_hw->timerawl;
        while(timer_hw->timerawl - 100000 < preTime){
            UseBallSensor();
        }
        //printf("end\n");
         printf("%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n",
         pulse[0],pulse[1],pulse[2],pulse[3],pulse[4],pulse[5],pulse[6],pulse[7],
         pulse[8],pulse[9],pulse[10],pulse[11],pulse[12],pulse[13],pulse[14],pulse[15],timer_hw->timerawl);
    }
}