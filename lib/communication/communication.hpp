#pragma once

#include "i2c_slave.h"
#ifdef __cplusplus
extern "C" {
#endif

void I2CSetup();
void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event);

#ifdef __cplusplus
}
#endif