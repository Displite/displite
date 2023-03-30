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

SET(USE_GUI "NULL" CACHE STRING "GUI to use")

execute_process(COMMAND python3 "${CMAKE_SOURCE_DIR}/configure.py" "gui_util" OUTPUT_VARIABLE AVAILABLE_GUI OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)

if(NOT USE_GUI IN_LIST AVAILABLE_GUI)
message(FATAL_ERROR "GUI: ${USE_GUI} is not a valid option.\nAvailable GUI's: ${AVAILABLE_GUI}")
endif()

execute_process(COMMAND python3 "${CMAKE_SOURCE_DIR}/configure.py" "gui_util" "path=${USE_GUI}" OUTPUT_VARIABLE USE_GUI_PATH OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_STRIP_TRAILING_WHITESPACE)

include("${USE_GUI_PATH}/${USE_GUI}.cmake")

message(STATUS "using GUI: ${USE_GUI}")

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
    ${CMAKE_CURRENT_LIST_DIR}/displite/src/images/*.c
)

add_executable(${EXE_NAME}
${MAIN_SRCS}
${GUI_SRCS}
)

string(TOUPPER "USE_${GUI_DISPLAY}" USE_DISPLAY_DRIVER)

target_compile_definitions(${EXE_NAME}
PUBLIC "${USE_DISPLAY_DRIVER}"
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