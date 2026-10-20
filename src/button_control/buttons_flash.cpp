#include <iostream>
#include <string>
#include "pico/stdlib.h"
#include "tusb.h"
#include "init/initialize.h"
#include "usb_descriptors.h"
#include "hardware/sync.h"
#include "hardware/flash.h"
#define FLASH_OFFSET (2*1024*1024-4096)
#define FLASH_SIZE 4096
#define BUTTON_COUNT 12
const int KEY_PIN[12] {1,2,3,4,5,6,7,8,9,10,11,12};
struct ButtonConfig {
    int ButtonPIN;
    char Type[12];
    char Function[16];
};

void save_config(const ButtonConfig *DATA_TO_SAVE, uint32_t data_size){
    uint8_t buffer[FLASH_SIZE]; // set the size of flash memory sector
    memset(buffer,0, FLASH_SIZE); // clear whole memory sector
    memcpy(buffer, DATA_TO_SAVE, data_size); // save data to memory sector
    uint32_t interrupts = save_and_disable_interrupts(); // disable interupts
    flash_range_erase(FLASH_OFFSET, FLASH_SIZE); // erase whole memory sector
    flash_range_program(FLASH_OFFSET, buffer ,FLASH_SIZE); // copy memory range that was setup few lines above
    restore_interrupts(interrupts); // restore interrupts
}

void load_config(const ButtonConfig *DATA_TO_LOAD, uint32_t data_size)
{
    const ButtonConfig *ReadFlash = (const ButtonConfig *)(XIP_BASE + FLASH_OFFSET);
    if(ReadFlash[0].ButtonPIN == KEY_PIN[0]){
        memcpy((void*)DATA_TO_LOAD, ReadFlash, data_size);
    }else{
        save_config(DATA_TO_LOAD, data_size);
    }
}

void assign_function(int id, int pin, std::string Type, std::string KeyFunction, ButtonConfig *ARRAY)
{
    int index = -1;
    if (id >=1 && id<=BUTTON_COUNT){
        index = id-1;
        if(pin > -1){
            ARRAY[index].ButtonPIN = pin;
        }
        if(Type != ""){
        strncpy(ARRAY[index].Type, Type.c_str(), sizeof(ARRAY[index].Type));
        ARRAY[index].Type[sizeof(ARRAY[index].Type) - 1] = '\0';
        }
        if(KeyFunction != ""){
        strncpy(ARRAY[index].Function, KeyFunction.c_str(), sizeof(ARRAY[index].Function));
        ARRAY[index].Function[sizeof(ARRAY[index].Function) - 1] = '\0';
        }
    }else{
        printf("Invalid button ID");
    }
}

void DefaultSettings(ButtonConfig *ARRAY){
    for(int i = 0; i<BUTTON_COUNT; i++){
    ARRAY[i].ButtonPIN = KEY_PIN[i];
    memset(ARRAY[i].Type, 0, sizeof(ARRAY[i].Type));
    memset(ARRAY[i].Function, 0, sizeof(ARRAY[i].Function));
    }
}