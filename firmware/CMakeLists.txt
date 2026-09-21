cmake_minimum_required(VERSION 3.13)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(telemetry C CXX ASM)
set(CMAKE_C_STANDARD 11)
pico_sdk_init()

add_executable(telemetry main.c ../src/ccsds.c)
target_include_directories(telemetry PRIVATE ../src)
target_link_libraries(telemetry pico_stdlib hardware_adc)

pico_enable_stdio_usb(telemetry 1)
pico_enable_stdio_uart(telemetry 0)
pico_add_extra_outputs(telemetry)