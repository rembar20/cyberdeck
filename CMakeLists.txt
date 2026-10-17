#Minimum CmakeVersion required
cmake_minimum_required(VERSION 3.13)
#import official SDK from the environment variable
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
#Set Project Name
project(CyberDeck C CXX ASM)

#Initialize the SDK
pico_sdk_init()

#Add executable. Default name is the project name, version 0.1
add_executable(CyberDeck
    src/main.cpp
    src/init/initialize.cpp
    src/serial_control/CommandHandler.cpp
    include/usb_descriptors.cpp
)

#Add libraries. Default is pico_stdlib, but you can also link other libraries
target_link_libraries(CyberDeck 
    pico_stdlib
    tinyusb_device
    tinyusb_board
)
# Add configuration folder
target_include_directories(CyberDeck PRIVATE
 include
)
pico_add_extra_outputs(CyberDeck)
