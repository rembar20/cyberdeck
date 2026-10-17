#include <iostream>
#include <string>
#include "pico/stdlib.h"
#include "tusb.h"
#include "init/initialize.h"
#include "serial_control/CommandHandler.h"

int main() {
	initialize();
	int character = 0;
	std::string command = "";
	
    while (true) {
	// Sprawdź, czy nadszedł jakiś znak (z timeoutem 0, żeby pętla się nie blokowała)
		CommandHandler(character, command);
        sleep_ms(10); // Krótka pauza, żeby nie obciążać procesora w 100%
	}
 // Do nothing, just keep the program running to maintain the USB connection
    }