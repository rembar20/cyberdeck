#ifndef CommandHandler_h
#define CommandHandler_h

#include <string>
struct ButtonConfig {
    int ButtonID;
    char Type[12];
    char Function[16];
};

const int KEY_PIN[12] {1,2,3,4,5,6,7,8,9,10,11,12};
void save_config(const ButtonConfig *DATA_TO_SAVE, uint32_t data_size);
void load_config(const ButtonConfig *DATA_TO_LOAD, uint32_t data_size);


#endif