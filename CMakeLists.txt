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

# Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

project(tarefa1-aula-sincrona-27-01-interrupcoes C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable
add_executable(tarefa1-aula-sincrona-27-01-interrupcoes tarefa1-aula-sincrona-27-01-interrupcoes.c)

# Generate PIO header
pico_generate_pio_header(tarefa1-aula-sincrona-27-01-interrupcoes ${CMAKE_CURRENT_LIST_DIR}/pio_led.pio)

# Set program name and version
pico_set_program_name(tarefa1-aula-sincrona-27-01-interrupcoes "tarefa1-aula-sincrona-27-01-interrupcoes")
pico_set_program_version(tarefa1-aula-sincrona-27-01-interrupcoes "0.1")

# Enable UART and USB
pico_enable_stdio_uart(tarefa1-aula-sincrona-27-01-interrupcoes 1)
pico_enable_stdio_usb(tarefa1-aula-sincrona-27-01-interrupcoes 1)

# Link libraries
target_link_libraries(tarefa1-aula-sincrona-27-01-interrupcoes
        pico_stdlib
        hardware_pio
)

# Include directories
target_include_directories(tarefa1-aula-sincrona-27-01-interrupcoes PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add extra outputs
pico_add_extra_outputs(tarefa1-aula-sincrona-27-01-interrupcoes)
