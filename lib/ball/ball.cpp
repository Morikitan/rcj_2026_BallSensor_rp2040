#include "ball.hpp"
#include "../config.hpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"
#include "hardware/pio.h"
#include "four_pin_monitoring.pio.h"

PIO pio = pio0;
uint offset,sm = 0;

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
    gpio_set_dir(Sensorpin0,GPIO_IN); gpio_set_dir(Sensorpin1,GPIO_IN);
    gpio_set_dir(Sensorpin2,GPIO_IN); gpio_set_dir(Sensorpin3,GPIO_IN);
    gpio_set_dir(Sensorpin4,GPIO_IN); gpio_set_dir(Sensorpin5,GPIO_IN);
    gpio_set_dir(Sensorpin6,GPIO_IN); gpio_set_dir(Sensorpin7,GPIO_IN);
    gpio_set_dir(Sensorpin8,GPIO_IN); gpio_set_dir(Sensorpin9,GPIO_IN);
    gpio_set_dir(Sensorpin10,GPIO_IN); gpio_set_dir(Sensorpin11,GPIO_IN);
    gpio_set_dir(Sensorpin12,GPIO_IN); gpio_set_dir(Sensorpin13,GPIO_IN);
    gpio_set_dir(Sensorpin14,GPIO_IN); gpio_set_dir(Sensorpin15,GPIO_IN);
    
    //four_pin_monitoringという名のプログラムをPIOの命令メモリに配置する
    offset = pio_add_program(pio,&four_pin_monitoring_program);

    //pioの各ステートマシンの初期設定
    FourPinMonitoringInit(pio,0,offset,Sensorpin0);
    FourPinMonitoringInit(pio,1,offset,Sensorpin4);
    FourPinMonitoringInit(pio,2,offset,Sensorpin8);
    FourPinMonitoringInit(pio,3,offset,Sensorpin12);
}

//ボールセンサー(赤外線センサー)を使う。
void UseBallSensor(){
    for(int sm = 0;sm <= 3;sm++){
        //pioのRX FIFOに何かがあるときだけ処理を行う。
        if (!pio_sm_is_rx_fifo_empty(pio, sm)) {

        }
    }
}

//pioの初期設定をする。
//
//pio : pio0かpio1がある。今回はpio0。
//sm : ステートマシン。0～3の4つある。
//offset : プログラムの開始位置
//FrontPin : 先頭のピン。3を選ぶと、3,4,5,6のピンを監視する。
void FourPinMonitoringInit(PIO pio, uint32_t sm, uint32_t offset,uint32_t FrontPin){
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

