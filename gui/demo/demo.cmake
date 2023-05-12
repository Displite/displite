set(GUI_DISPLAY "ili9486")
set(GUI_VERSION "0.1")


# Do not modify
file(GLOB_RECURSE GUI_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

include_directories(
"${CMAKE_CURRENT_LIST_DIR}"
)
