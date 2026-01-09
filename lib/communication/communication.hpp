#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void I2CSetup();
// void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);
void picoPioUartTx_program_putc(unsigned char c, bool even_parity);
unsigned char picoPioUartRx_program_getc(bool even_parity,bool* parity_check);
void Callback();

#ifdef __cplusplus
}
#endif