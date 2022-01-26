# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.22.0)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
file(TO_CMAKE_PATH "$ENV{FREERTOS_KERNEL_PATH}" FREERTOS_PATH)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(MacroPad C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Add libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/src/lib/WS2812)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/lib/Encoder)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/src/MacroPad.cpp
    ${CMAKE_SOURCE_DIR}/src/lib/tinyusb/usb_descriptors.c
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/lib/tinyusb
)

# FreeRTOS include
add_library(FreeRTOS STATIC
    ${FREERTOS_PATH}/tasks.c
    ${FREERTOS_PATH}/queue.c
    ${FREERTOS_PATH}/list.c
    ${FREERTOS_PATH}/portable/GCC/ARM_CM0/port.c
    ${FREERTOS_PATH}/portable/MemMang/heap_4.c
    ${FREERTOS_PATH}/timers.c
    ${FREERTOS_PATH}/event_groups.c
    ${FREERTOS_PATH}/stream_buffer.c
    ${FREERTOS_PATH}/croutine.c
)

target_include_directories(FreeRTOS PUBLIC
    ${FREERTOS_PATH}/include
    ${FREERTOS_PATH}/portable/GCC/ARM_CM0
)

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
    WS2812
    Encoder
    tinyusb_device
    tinyusb_board
    FreeRTOS
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})
