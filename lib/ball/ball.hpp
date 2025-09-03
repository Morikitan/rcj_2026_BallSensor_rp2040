#pragma once

#include "hardware/pio.h"

#ifdef __cplusplus
extern "C" {
#endif

void BallSetup();
void UseBallSensor();
void TwoPinMonitoringInit(PIO pio, uint32_t sm, uint32_t offset,uint32_t FrontPin);

#ifdef __cplusplus
}
#endif