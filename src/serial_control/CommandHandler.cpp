#include <iostream>
#include <string>
#include "pico/stdlib.h"
#include "tusb.h"
#include "init/initialize.h"
#include "usb_descriptors.h"
#include "pico/bootrom.h"
#include "button_control/buttons_flash.h"


uint16_t StringToHex(const std::string &str);
void commandHelp(const std::string &CommandArgument);
void commandPing(const std::string &CommandArgument);
void commandEcho(const std::string &CommandArgument);
void commandMedia(const std::string &CommandArgument);
void commandKeyboard(const std::string &CommandArgument);
void firmware_update_mode(const std::string &CommandArgument);
void commandMute(const std::string &CommandArgument);
void commandVolUp(const std::string &CommandArgument);
void commandVolDown(const std::string &CommandArgument);
void commandSet(const std::string &CommandArgument);
void commandLoadConfig(const std::string &CommandArgument);
void commandSaveConfig(const std::string &CommandArgument);
void commandFactoryReset(const std::string &CommandArgument);
std::string command = "";
std::string command_argument = "";
uint16_t release_key = 0x0000;
struct CommandList{
    std::string command;
    void (*function)(const std::string &CommandArgument); // Function pointer with default argument
    std::string description;
};

CommandList commandList[] = {
    {"help", commandHelp, "Display this help message"},
    {"ping", commandPing, "Connectivity Test"},
    {"echo", commandEcho, "Echo the input"},
    {"media", commandMedia, "Control media keys, use Hex code as an argument, e.g., media 0xE2 for Mute"},
    {"keyboard", commandKeyboard, "Send keyboard keys, use Hex code as an argument, e.g., keyboard 0x04 for 'a'"},
    {"firmware_update", firmware_update_mode, "Reboot into BOOTSEL mode for firmware update"},
    {"mute", commandMute, "Mute the system volume"},
    {"volup", commandVolUp, "Increase the system volume"},
    {"voldown", commandVolDown, "Decrease the system volume"},
    {"set", commandSet, "Assign a function to a key"},
    {"load_config", commandLoadConfig, "Load config from flash memory"},
    {"save_config", commandSaveConfig, "Save config to flash memory"},
    {"ResetToDefault", commandFactoryReset, "RESET FLASH MEMORY TO DEFAULT SETTINGS!"}

};

void CommandHandler(int character, std::string &recieved_command){
    character = getchar_timeout_us(0);
    if (character != PICO_ERROR_TIMEOUT && character >= 32 && character <= 126) { // if a character is received and it is a printable character
        recieved_command += char(character); // add character to command
    } else if (character == '\b' || character == 127) { // on backspace
        if (!recieved_command.empty()) {
            recieved_command.pop_back(); // remove last character from command
        }
    } else if (character == '\n' || character == '\r') { // if new line or carriage return is received, execute the command
        if (!recieved_command.empty()) {
            size_t comm_end_pos = recieved_command.find(' ');
            command = recieved_command.substr(0, comm_end_pos); // extract command
            if(comm_end_pos != std::string::npos){command_argument = recieved_command.substr(comm_end_pos + 1);}else{command_argument = "";}; // extract argument
            bool found = false;
            for (unsigned int i = 0; i < sizeof(commandList)/sizeof(commandList[0]); ++i) {
                if (command == commandList[i].command) {
                    printf("Executing command: %s\n", command.c_str());
                    commandList[i].function(command_argument); // execute the corresponding function
                    recieved_command.clear(); // Clear command buffer after execution
                    found = true;
                    break;
                }
            }
            if (!found) {
            printf("Unknown command: %s Type 'help' for available commands\n", command.c_str());
            recieved_command.clear(); // Clear command buffer after execution
            }
        } 
    }
}

void commandHelp(const std::string &CommandArgument){
    printf("Available commands:\n");
    for (const auto& cmd : commandList) {
        printf("- %-10s %-5s\n", cmd.command.c_str(), cmd.description.c_str());
    }
}

void commandPing(const std::string &CommandArgument){
    printf("Pong!\n");
}

void commandEcho(const std::string &CommandArgument){
    printf("%s\n", CommandArgument.c_str());
}

void commandMedia(const std::string &CommandArgument){
    
    uint16_t Keycode = StringToHex(CommandArgument);
    if(tud_mounted()){
      tud_hid_report(REPORT_ID_CONSUMER, &Keycode, sizeof(Keycode)); // Send key press report
        sleep_ms(100); // Optional delay to ensure the host is ready
      tud_hid_report(REPORT_ID_CONSUMER, &release_key, sizeof(release_key)); // Send key release report
    }
}


void commandKeyboard(const std::string &CommandArgument){
    uint16_t Keycode = StringToHex(CommandArgument);
    if(tud_mounted()){
      tud_hid_report(REPORT_ID_KEYBOARD, &Keycode, sizeof(Keycode)); // Send key press report
        sleep_ms(100); // Optional delay to ensure the host is ready
      tud_hid_report(REPORT_ID_KEYBOARD, &release_key, sizeof(release_key)); // Send key release report
    }
}

uint16_t StringToHex(const std::string &str) {
    uint16_t keycode = 0x0000;
    char *endptr = nullptr;
        unsigned long keycode_long = std::strtoul(str.c_str(), &endptr, 16); // Convert hex string to unsigned long
    if (endptr == str.c_str() || *endptr != '\0') {
        printf("Invalid argument. Please provide a valid hex code.\n");
        return 0x0000;
    } else {
        keycode = static_cast<uint16_t>(keycode_long);
        printf("Converted hex code: 0x%04X\n", keycode);
    }
    return keycode;
}

void firmware_update_mode(const std::string &CommandArgument) {
    printf("Przechodze w tryb BOOTSEL...\n");
    sleep_ms(100);
    // Funkcja Pico SDK wywołująca natychmiastowy tryb dysku USB (BOOTSEL)
    reset_usb_boot(0, 0);
}

void commandMute(const std::string &CommandArgument) {
    commandMedia("0xE2"); // Mute keycode
}
void commandVolUp(const std::string &CommandArgument) {
    commandMedia("0xE9"); // Volume Up keycode
}
void commandVolDown(const std::string &CommandArgument) {
    commandMedia("0xEA"); // Volume Down keycode
}