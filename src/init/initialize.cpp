    #include <stdio.h>
    #include "pico/stdlib.h"
    #include "tusb.h"
    #include "initialize.h"
    
    void initialize() {
    //Initialize Standard I/O and TinyUSB
	stdio_init_all();
	tusb_init();
    
	// Wait for the USB connection to be established
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    // Wait for a moment to ensure the USB connection is stable
    sleep_ms(500);
	// 
    printf("MCU Initialized\n");
}