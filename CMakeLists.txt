cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include(clib/pico-sdk/pico_sdk_init.cmake)

project(displite 
VERSION 0.3
DESCRIPTION "Status information via USB HID"
HOMEPAGE_URL "https://github.com/displite/displite"
)

include(cmake/helpers.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(
"${CMAKE_CURRENT_LIST_DIR}/displite/includes"
"${CMAKE_CURRENT_LIST_DIR}/clib"
)

SET(USE_GUI "demo" CACHE STRING "GUI to use")

include("${CMAKE_CURRENT_LIST_DIR}/gui/${USE_GUI}/${USE_GUI}.cmake")

message(STATUS "Using GUI: ${USE_GUI}")

set(EXE_NAME "displite_${USE_GUI}")

add_subdirectory(clib/lvgl)

target_compile_definitions(lvgl PUBLIC LV_CONF_INCLUDE_SIMPLE)

file(GLOB_RECURSE MAIN_SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/*.cpp 
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/*.c
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/displays/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/images/*.c
)

add_executable(${EXE_NAME}
${MAIN_SRCS}
${GUI_SRCS}
)

proper_case(${USE_GUI} PRODUCT_NAME)

string(TOUPPER "USE_${GUI_DISPLAY}" USE_DISPLAY_DRIVER)

target_compile_definitions(${EXE_NAME}
PUBLIC "${USE_DISPLAY_DRIVER}"
PUBLIC PRODUCT_NAME="${PRODUCT_NAME}"
PUBLIC DISPLITE_INFORMATION="${CMAKE_PROJECT_VERSION}\\\;${GUI_VERSION}"
PUBLIC PICO_NO_BINARY_INFO=1
PUBLIC GUI_HEADER="${USE_GUI}.h"
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

pico_enable_stdio_uart(${EXE_NAME} 1)
pico_enable_stdio_usb(${EXE_NAME} 0)

pico_add_extra_outputs(${EXE_NAME})