cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(clib/pico-sdk/pico_sdk_init.cmake)

project(displite 
VERSION 0.1
DESCRIPTION "Status information via USB HID"
HOMEPAGE_URL "https://github.com/displite/displite"
)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(displite_gui
${CMAKE_CURRENT_LIST_DIR}/displite/includes
${CMAKE_CURRENT_LIST_DIR}/portar/includes
${CMAKE_CURRENT_LIST_DIR}/clib
)

add_subdirectory(clib/lvgl)

target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE)

file(GLOB_RECURSE MAIN_SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/*.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/*.c
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/displays/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/portar/src/*.c
    ${CMAKE_CURRENT_LIST_DIR}/portar/src/*.cpp
)

add_executable(displite_gui
${MAIN_SRCS}
)

target_compile_definitions(displite_gui
PUBLIC USE_ILI9486
)

target_link_libraries(displite_gui
pico_stdlib
pico_multicore
pico_time
tinyusb_device
pico_unique_id
hardware_spi
lvgl
lvgl_demos
)