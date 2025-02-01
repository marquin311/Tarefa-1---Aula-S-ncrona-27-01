# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
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

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(0a9 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable
add_executable(0a9 0a9.c ws2812.c)

pico_set_program_name(0a9 "0a9")
pico_set_program_version(0a9 "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(0a9 0)
pico_enable_stdio_usb(0a9 0)

# Link libraries: adicione hardware_pio para funções de PIO
target_link_libraries(0a9
        pico_stdlib
        hardware_pio
)

# Adiciona os diretórios de inclusão
target_include_directories(0a9 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        "C:/Users/Marcos/Documents/testes/0a9/build/generated"
)

pico_add_extra_outputs(0a9)
