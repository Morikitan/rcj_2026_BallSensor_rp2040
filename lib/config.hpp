#pragma once

#include "hardware/sync.h" 
//2つのコアで共有の変数にはvolatileをつける

//赤外線センサ
#define Sensorpin0 6
#define Sensorpin1 7
#define Sensorpin2 8
#define Sensorpin3 9
#define Sensorpin4 10
#define Sensorpin5 11
#define Sensorpin6 12
#define Sensorpin7 13
#define Sensorpin8 22
#define Sensorpin9 23
#define Sensorpin10 24
#define Sensorpin11 25
#define Sensorpin12 2
#define Sensorpin13 3
#define Sensorpin14 4
#define Sensorpin15 5
extern volatile uint16_t pulse[16];

//I2C通信
#define SDApin 0
#define SCLpin 1

//マルチコア
extern spin_lock_t *lock;