    #include <stdio.h>
    #include "pico/stdlib.h"
    #include "tusb.h"
    #include "initialize.h"
    void initialize(int *PinArray) {
    //Initialize Standard I/O and TinyUSB
	stdio_init_all();
	tusb_init();
    for (int i = 0; i < 12; i++) {
        gpio_init(PinArray[i]);
        gpio_set_dir(PinArray[i], GPIO_IN);
        gpio_pull_up(PinArray[i]); // Enable pull-up resistor
    }
    // Wait for a moment to ensure the USB connection is stable
    sleep_ms(500);
	// 
    printf("MCU Initialized\n");
}