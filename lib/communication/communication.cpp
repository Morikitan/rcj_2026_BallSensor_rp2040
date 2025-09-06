#include "communication.hpp"
#include "../config.hpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "i2c_slave.h"
#include "pico/multicore.h"
#include "pico/sync.h"

uint8_t buffer[32];
uint16_t saved_irq;

void I2CSetup(){
    gpio_init(SDApin);
    gpio_init(SCLpin);
    gpio_set_function(SDApin,GPIO_FUNC_I2C);
    gpio_set_function(SCLpin,GPIO_FUNC_I2C);
    //外部プルアップをする
    i2c_slave_init(i2c0,0x42,&i2c_slave_handler);
    i2c_init(i2c0,100000);
}

void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event){
    switch (event) {
    case I2C_SLAVE_RECEIVE: 
        // マスターからデータを受け取った時の処理(現在はなし)
        break;
    case I2C_SLAVE_REQUEST: 
        // マスターからデータを送信させられた時の処理
        spin_lock_blocking(lock);
        for(int i = 0;i < 16;i++){
            buffer[i * 2] = (pulse[i] >> 8) & 0xFF;//上位8ビット
            buffer[i * 2 + 1] = pulse[i] & 0xFF;//下位8ビット
        }
        spin_unlock(lock,saved_irq);
        i2c_write_blocking(i2c,0x42,buffer,32,false);
        break;
    case I2C_SLAVE_FINISH:
        // マスターからstopかrestartが来た時の処理(今はなし)
        break;  
    default:
        break;
    }
}