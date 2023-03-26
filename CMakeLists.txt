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

include(portar/portar.cmake)

set(EXE_NAME "displite_${GUI_NAME}")

include_directories(
"${CMAKE_CURRENT_LIST_DIR}/displite/includes"
"${CMAKE_CURRENT_LIST_DIR}/clib"
)

add_subdirectory(clib/lvgl)

target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE)

file(GLOB_RECURSE MAIN_SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/*.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/*.c
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/displays/*.cpp
)

add_executable(${EXE_NAME}
${MAIN_SRCS}
${GUI_SRCS}
)

string(TOUPPER ${GUI_DISPLAY} USE_DISPLAY_DRIVER)

target_compile_definitions(${EXE_NAME}
PUBLIC "USE_${USE_DISPLAY_DRIVER}"
PUBLIC PRODUCT_NAME="${GUI_NAME}"
)

target_link_libraries(${EXE_NAME}
pico_stdlib
pico_multicore
pico_time
tinyusb_device
pico_unique_id
hardware_spi
hardware_watchdog
lvgl
)