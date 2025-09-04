#pragma once

#include "hardware/pio.h"

#ifdef __cplusplus
extern "C" {
#endif

void BallSetup();
void UseBallSensor();
void PIOPinMonitoringInit(PIO pio, uint32_t sm, uint32_t offset,uint32_t FrontPin);
void BallSensorFallOrRise(uint gpio, uint32_t events);

#ifdef __cplusplus
}
#endif