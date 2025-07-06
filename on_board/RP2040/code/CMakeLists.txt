cmake_minimum_required(VERSION 3.13...3.27)
set(ENV{PICO_DEFAULT_BINARY_TYPE} "no_flash")
include(./Core/Src/pico-sdk/external/pico_sdk_import.cmake)
include_directories(./Core/Inc/)
project(microfuzz C CXX ASM)
pico_sdk_init()

add_executable(microfuzz ./Core/Src/main.c ./Core/Src/fuzz/fuzz.c ./Core/Src/fuzz/harness.c ./Core/Src/fuzz/time_bare.c ./Core/Src/fuzz/coverage.S)
target_link_libraries(microfuzz pico_stdlib hardware_gpio)
