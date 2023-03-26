set(GUI_NAME "portar")
set(GUI_DISPLAY "ili9486")


# Do not modify
file(GLOB_RECURSE GUI_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/*.c
    ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
)

include_directories(
"${CMAKE_CURRENT_LIST_DIR}/includes"
)