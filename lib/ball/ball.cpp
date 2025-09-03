#include "ball.hpp"
#include "../config.hpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"
#include "hardware/pio.h"
#include "two_pin_monitoring.pio.h"

uint32_t offset0,offset1,sm = 0;
uint32_t pretime[8] = {0,0,0,0,0,0,0,0};
uint32_t data;

//ボールセンサーの初期化
void BallSetup(){
    volatile int pulse[8] = {0,0,0,0,0,0,0,0};

    gpio_init(Sensorpin0); gpio_init(Sensorpin1);
    gpio_init(Sensorpin2); gpio_init(Sensorpin3);
    gpio_init(Sensorpin4); gpio_init(Sensorpin5);
    gpio_init(Sensorpin6); gpio_init(Sensorpin7);
    gpio_init(Sensorpin8); gpio_init(Sensorpin9);
    gpio_init(Sensorpin10); gpio_init(Sensorpin11);
    gpio_init(Sensorpin12); gpio_init(Sensorpin13);
    gpio_init(Sensorpin14); gpio_init(Sensorpin15);
    gpio_set_dir(Sensorpin0,GPIO_IN); gpio_set_dir(Sensorpin1,GPIO_IN);
    gpio_set_dir(Sensorpin2,GPIO_IN); gpio_set_dir(Sensorpin3,GPIO_IN);
    gpio_set_dir(Sensorpin4,GPIO_IN); gpio_set_dir(Sensorpin5,GPIO_IN);
    gpio_set_dir(Sensorpin6,GPIO_IN); gpio_set_dir(Sensorpin7,GPIO_IN);
    gpio_set_dir(Sensorpin8,GPIO_IN); gpio_set_dir(Sensorpin9,GPIO_IN);
    gpio_set_dir(Sensorpin10,GPIO_IN); gpio_set_dir(Sensorpin11,GPIO_IN);
    gpio_set_dir(Sensorpin12,GPIO_IN); gpio_set_dir(Sensorpin13,GPIO_IN);
    gpio_set_dir(Sensorpin14,GPIO_IN); gpio_set_dir(Sensorpin15,GPIO_IN);
    
    //four_pin_monitoringという名のプログラムをPIOの命令メモリに配置する
    offset0 = pio_add_program(pio0,&four_pin_monitoring_program);
    offset1 = pio_add_program(pio1,&four_pin_monitoring_program);

    //pioの各ステートマシンの初期設定
    TwoPinMonitoringInit(pio0,0,offset0,Sensorpin0);
    TwoPinMonitoringInit(pio0,1,offset0,Sensorpin2);
    TwoPinMonitoringInit(pio0,2,offset0,Sensorpin4);
    TwoPinMonitoringInit(pio0,3,offset0,Sensorpin6);
    TwoPinMonitoringInit(pio1,0,offset1,Sensorpin8);
    TwoPinMonitoringInit(pio1,1,offset1,Sensorpin10);
    TwoPinMonitoringInit(pio1,2,offset1,Sensorpin12);
    TwoPinMonitoringInit(pio1,3,offset1,Sensorpin14);
}

//ボールセンサー(赤外線センサー)を使う。
void UseBallSensor(){
    for(int sm = 0;sm <= 3;sm++){
        //pioのRX FIFOに何かがあるときだけ処理を行う。
        if (!pio_sm_is_rx_fifo_empty(pio0, sm)) {
            //RX FIFOからデータを受け取る。
            data = pio_sm_get(pio0,sm);
        }
    }
    for(int sm = 0;sm <= 3;sm++){
        //pioのRX FIFOに何かがあるときだけ処理を行う。
        if (!pio_sm_is_rx_fifo_empty(pio1, sm)) {

        }
    }
}

//pioの初期設定をする。
//
//pio : pio0かpio1がある。今回はpio0。
//sm : ステートマシン。0～3の4つある。
//offset : プログラムの開始位置ビット(あまり気にしなくてよい)
//FrontPin : 先頭のピン。3を選ぶと、3,4,5,6のピンを監視する。
void TwoPinMonitoringInit(PIO pio, uint32_t sm, uint32_t offset,uint32_t FrontPin){
    //配置したプログラムの初期設定を取得する
    pio_sm_config c = four_pin_monitoring_program_get_default_config(offset);
    //連続したFrontPin～FrontPin+3 のピンをin pins命令の対象に設定
    sm_config_set_in_pins(&c,FrontPin);
    //PIOのステートマシンの動作速度を設定(この場合は最速)
    sm_config_set_clkdiv(&c,1.0f);
    //PIOのステートマシンの初期化
    pio_sm_init(pio, sm, offset, &c);
    //PIOのステートマシンを有効化
    pio_sm_set_enabled(pio, sm, true);
}

