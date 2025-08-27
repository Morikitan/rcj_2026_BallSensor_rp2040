#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ball/ball.hpp"
#include "communication/communication.hpp"

int main()
{
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
