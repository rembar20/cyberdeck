#ifndef CommandHandler_h
#define CommandHandler_h

#include <string>
struct ButtonConfig {
    int ButtonID;
    char Type[12];
    char Function[16];
};


void save_config(const ButtonConfig *DATA_TO_SAVE, uint32_t data_size);
void load_config(const ButtonConfig *DATA_TO_LOAD, uint32_t data_size);


#endif