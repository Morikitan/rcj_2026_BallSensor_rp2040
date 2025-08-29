#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void BallSetup();
void UseBallSensor();
void PIO_UseBallSensor();
void Interrupt_UseBallSensor();

#ifdef __cplusplus
}
#endif