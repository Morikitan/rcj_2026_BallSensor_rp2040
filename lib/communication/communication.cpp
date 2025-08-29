#include "communication.hpp"
#include "../config.hpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "i2c_slave.h"

void I2CSetup(){
    gpio_init(SDApin);
    gpio_init(SCLpin);
    gpio_set_function(SDApin,GPIO_FUNC_I2C);
    gpio_set_function(SCLpin,GPIO_FUNC_I2C);
    //i2c_slave_init(i2c0,true,&i2c_slave_handler);
    i2c_init(i2c0,400000);
}

void UseI2C(){
    
}