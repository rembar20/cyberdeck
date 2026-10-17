#include <iostream>
#include <string>
#include "pico/stdlib.h"
#include "tusb.h"
#include "init/initialize.h"
#include "serial_control/CommandHandler.h"
#include "usb_descriptors.h"

int main() {
	initialize(); // USB and stdio initialization
	int character = 0; 
	std::string command = "";
	
    while (true) {
		tud_task();
		CommandHandler(character, command); // Function to handle command input and execution
        sleep_ms(10); // Pause for a short time to avoid busy waiting and allow other tasks to run
	}
    }