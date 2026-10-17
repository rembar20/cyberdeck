#include <iostream>
#include <string>
#include "pico/stdlib.h"
#include "tusb.h"
#include "init/initialize.h"

void commandHelp(const std::string &CommandArgument);
void commandPing(const std::string &CommandArgument);
void commandEcho(const std::string &CommandArgument);
std::string command = "";
std::string command_argument = "";
struct CommandList{
    std::string command;
    void (*function)(const std::string &CommandArgument); // Function pointer with default argument
    std::string description;
};

CommandList commandList[] = {
    {"help", commandHelp, "Display this help message"},
    {"ping", commandPing, "Connectivity Test"},
    {"echo", commandEcho, "Echo the input"},
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

