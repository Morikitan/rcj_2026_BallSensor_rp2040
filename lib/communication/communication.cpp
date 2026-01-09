#include "communication.hpp"
#include "../config.hpp"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "picoPioUart.pio.h"
#include "pico/multicore.h"
#include "pico/sync.h"

uint8_t buffer[32];

PIO pio;
uint sm_rx;
uint sm_tx;
uint offset;
uint offset2;
bool parity_check;

#define SERIAL_BAUD 125000

void I2CSetup(){
    pio = pio0;

    sm_rx = 0;

    //使うピン番号が32以上なので必要な処理群

    offset = pio_add_program(pio, &picoPioUartRx_program);
    picoPioUartRx_program_init(pio, sm_rx, offset, (uint)(SDApin), SERIAL_BAUD);

    // 使うSMを指定する(送信と受信では別のSMを使う)
    sm_tx = 1;
    offset2 = pio_add_program(pio, &picoPioUartTx_program);
    picoPioUartTx_program_init(pio, sm_tx, offset2, (uint)(SCLpin), SERIAL_BAUD);

    gpio_set_irq_enabled(SDApin,GPIO_IRQ_EDGE_FALL,true);
}


void Callback(){
    picoPioUartRx_program_getc(true,&parity_check);
    for(int i = 0;i <= 31;i++){
        picoPioUartTx_program_putc(buffer[i],true);
    }
}
//UART(シリアル通信)で送信する関数
//
//data : 送るデータ(uint8_t型)
//even_parity : 偶数か奇数のどちらになるようにパリティを付加するか。trueで偶数。falseで奇数。
void picoPioUartTx_program_putc(unsigned char data, bool even_parity) {
    uint32_t byte = (uint32_t)data;
    uint8_t parity = 0;
    for (int i = 0; i < 8; i++) {
        parity ^= byte & 0x1;
        byte >>= 1;
    }
    byte = (uint32_t)data;
    if (parity) {
        if (even_parity) {
            byte |= 0x100;  // 偶数になるようにパリティを付加します
        }
    } else {
        if (!even_parity) {
            byte |= 0x100;  // 奇数になるようにパリティを付加します
        }
    }
    pio_sm_put_blocking(pio, sm_tx, (uint32_t)byte);  // TX FIFOへputします
}

//UART(シリアル通信)で受信する関数
//
//
//even_parity : 偶数か奇数のどちらになるようにパリティを付加されているか。trueで偶数。falseで奇数。
//parity_check : パリティビットの結果。正しいならtrue。違ったらfalseで、例外処理を用意する。データがなくてもfalseになる。
unsigned char picoPioUartRx_program_getc(bool even_parity,bool* parity_check) {
    // if(pio_sm_is_rx_fifo_empty(pio, sm_rx)){
        // *parity_check = false;
        // return 0;
    // }else{
     while (pio_sm_is_rx_fifo_empty(pio, sm_rx)) {
        tight_loop_contents();
        // printf("待機中");
     }
     
    uint32_t c32 = pio_sm_get(pio, sm_rx);

    c32 >>= 23;
    //パリティビットの検証をする
    bool real_parity = (c32 & 0x100) != 0;
    uint8_t byte = c32 & 0xff;

    uint8_t pcheck = 0;
    for (int i = 0; i < 8; i++) {
        pcheck ^= byte & 0x1;
        byte >>= 1;
    }

    *parity_check = (pcheck == real_parity);

    return (unsigned char)(c32 & 0xff);
    // }
}