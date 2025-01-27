# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech main.c )

pico_set_program_name(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech "Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech")
pico_set_program_version(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech "0.1")

# # Generate PIO header
pico_generate_pio_header(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech ${CMAKE_CURRENT_LIST_DIR}/pios/ws2812.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech 1)
pico_enable_stdio_usb(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech 1)

# Add the standard library to the build
target_link_libraries(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech 
        hardware_pio
        )

pico_add_extra_outputs(Debolcing_Leds_e_Botoes_U4C4-EmbarcaTech)

