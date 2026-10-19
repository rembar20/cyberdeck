#include <iostream>
#include <string>
#include "pico/stdlib.h"
#include "tusb.h"
#include "init/initialize.h"
#include "serial_control/CommandHandler.h"
#include "usb_descriptors.h"
int BUTTON_PIN[12] = {18, 19, 20, 21, 6, 7, 8, 9, 10, 11, 12, 13}; // GPIO pins for buttons 
void BUTTON_FUNCTION(int pin,bool media, std::string keycode) {
	if(gpio_get(pin) == 0){
		if(tud_mounted){
			if(media){
			commandMedia(keycode);
			sleep_ms(200);
			}else
			commandKeyboard(keycode);
			sleep_ms(200);
		}
	}
}

int main() {
	initialize(BUTTON_PIN); // USB and stdio initialization
	int character = 0; 
	std::string command = "";
	
    while (true) {
		tud_task();
		CommandHandler(character, command); // Function to handle command input and execution
		// troll line commandMedia("0x70"); // Function to handle media commands
		BUTTON_FUNCTION(18,1,"0xE9");
		BUTTON_FUNCTION(19,1,"0xEA");
		BUTTON_FUNCTION(20,1,"0xE2");
		
        sleep_ms(10); // Pause for a short time to avoid busy waiting and allow other tasks to run
	}
    }