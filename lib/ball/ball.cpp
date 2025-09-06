#include "ball.hpp"
#include "../config.hpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio.h"
#include "hardware/pio.h"
#include "pin_monitoring.pio.h"
#include "pico/multicore.h"
#include "pico/sync.h"

uint32_t offset0,offset1,sm = 0;
uint32_t pretime[8] = {0,0,0,0,0,0,0,0};
uint32_t data;

//ボールセンサーの初期化
void BallSetup(){
    spin_lock_unsafe_blocking(lock);
    spin_unlock_unsafe(lock);

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
    
    //以下Sensorpin0～7 (割り込みかSIOで処理) ← 今は割り込みで処理
    //割り込みの初期設定
    gpio_set_irq_enabled_with_callback(Sensorpin0,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);
    gpio_set_irq_enabled_with_callback(Sensorpin1,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);
    gpio_set_irq_enabled_with_callback(Sensorpin2,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);
    gpio_set_irq_enabled_with_callback(Sensorpin3,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);
    gpio_set_irq_enabled_with_callback(Sensorpin4,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);
    gpio_set_irq_enabled_with_callback(Sensorpin5,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);
    gpio_set_irq_enabled_with_callback(Sensorpin6,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);
    gpio_set_irq_enabled_with_callback(Sensorpin7,GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,true,&BallSensorFallOrRise);

    //以下Sensorpin8～15 (pioで処理)
    //pin_monitoringという名のプログラムをPIOの命令メモリに配置する
    offset0 = pio_add_program(pio0,&pin_monitoring_program);
    offset1 = pio_add_program(pio1,&pin_monitoring_program);

    //pioの各ステートマシンの初期設定
    PIOPinMonitoringInit(pio0,0,offset0,Sensorpin8);
    PIOPinMonitoringInit(pio0,1,offset0,Sensorpin9);
    PIOPinMonitoringInit(pio0,2,offset0,Sensorpin10);
    PIOPinMonitoringInit(pio0,3,offset0,Sensorpin11);
    PIOPinMonitoringInit(pio1,0,offset1,Sensorpin12);
    PIOPinMonitoringInit(pio1,1,offset1,Sensorpin13);
    PIOPinMonitoringInit(pio1,2,offset1,Sensorpin14);
    PIOPinMonitoringInit(pio1,3,offset1,Sensorpin15);

    sleep_ms(1);

    for (int sm = 0;sm <= 3;sm++) pio_sm_put_blocking(pio0,sm,0xFFFFFFFF);
    for (int sm = 0;sm <= 3;sm++) pio_sm_put_blocking(pio1,sm,0xFFFFFFFF);
}

//ボールセンサー(赤外線センサー)を使う。
void UseBallSensor(){
    /*//以下Sensorpin0～7 (SIOで処理する場合)
    for (int pin = 6;pin <= 13;pin++){
        if(sio_hw->gpio_in & (1 << pin)){
           if (pretime[pin] == 0) {
                // 初めてHIGHになった時刻を記録
                pretime[pin] = timer_hw->timerawl;
            } 
        }else{
            if (pretime[pin] != 0) {
                // LOWになった → HIGHの時間を計算
                pulse[pin] = timer_hw->timerawl - pretime[pin]; // [us]
                pretime[pin] = 0; // リセットして次のHIGHに備える
            }
        }
    }
    */
    //以下Sensorpin8～15 (pioで処理)
    for(int sm = 0;sm <= 3;sm++){
        //pioのRX FIFOに何かがあるときだけ処理を行う。
        if (!pio_sm_is_rx_fifo_empty(pio0, sm)) {
            //RX FIFOからデータを受け取る。
            data = pio_sm_get(pio0,sm);
            //ステートマシンに時間計測用の基準の数値を送る
            pio_sm_put_blocking(pio0,sm,0xFFFFFFFF);
            //pulse[μs]の計算。経過時間は0xFFFFFFFFからの引き算であらわされ、
            //1減るごとに40[ns] = 0.04[μs]経過していることを表す
            pulse[8 + sm] = (int)((0xFFFFFFFF - data) * 0.04);
        }
    }
    for(int sm = 0;sm <= 3;sm++){
        //pioのRX FIFOに何かがあるときだけ処理を行う。
        if (!pio_sm_is_rx_fifo_empty(pio1, sm)) {
            data = pio_sm_get(pio1,sm);
            //ステートマシンに時間計測用の基準の数値を送る
            pio_sm_put_blocking(pio1,sm,0xFFFFFFFF);
            //pulse[μs]の計算。経過時間は0xFFFFFFFFからの引き算であらわされ、
            //1減るごとに40[ns] = 0.04[μs]経過していることを表す
            pulse[12 + sm] = (int)((0xFFFFFFFF - data) * 0.04);
        }
    }
}

//pioの初期設定をする。
//
//pio : pio0かpio1がある。今回はpio0。
//sm : ステートマシン。0～3の4つある。
//offset : プログラムの開始位置ビット(あまり気にしなくてよい)
//pin : 監視するピンの番号
void PIOPinMonitoringInit(PIO pio, uint32_t sm, uint32_t offset,uint32_t pin){
    //配置したプログラムの初期設定を取得する
    pio_sm_config c = pin_monitoring_program_get_default_config(offset);
    //FrontPin～FrontPin+3 のピンをin pins命令の対象に設定
    sm_config_set_in_pins(&c,pin);
    //PIOのステートマシンの動作速度を設定(この場合は最速)
    sm_config_set_clkdiv(&c,1.0f);
    //PIOのステートマシンの初期化
    pio_sm_init(pio, sm, offset, &c);
    //PIOのステートマシンを有効化
    pio_sm_set_enabled(pio, sm, true);
}

//割り込み時に実行される関数
//
//gpio : 割り込みが起きたgpio(6～13)
//events : 割り込みの要因(GPIO_IRQ_EDGE_FALLかGPIO_IRQ_EDGE_RISE)
void BallSensorFallOrRise(uint gpio, uint32_t events){
    if(events == GPIO_IRQ_EDGE_FALL){
        //HIGHからLOWの処理
        spin_lock_unsafe_blocking(lock);
        if(timer_hw->timerawl - pretime[gpio - 6] > 65535){
            //オーバーフローを防止する
            pulse[gpio - 6] = 65535;
        }else{
            pulse[gpio - 6] = timer_hw->timerawl - pretime[gpio - 6];
        }
        spin_unlock_unsafe(lock);
    }else{
        //LOWからHIGHの処理
        pretime[gpio - 6] = timer_hw->timerawl;
    } 
}



