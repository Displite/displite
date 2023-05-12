set(GUI_VERSION "{{ cookiecutter.project_version }}")


# Do not modify
file(GLOB_RECURSE GUI_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.c
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

include_directories(
"${CMAKE_CURRENT_LIST_DIR}"
)
