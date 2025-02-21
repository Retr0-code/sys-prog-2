cmake_minimum_required(VERSION 3.17.0)

project(utility VERSION 1.0)

set(DIR_SOURCES src)
set(DIR_INCLUDE include)

add_library(${PROJECT_NAME}_objects OBJECT
    src/help.c
    src/users.c
    src/processes.c
)

target_include_directories(${PROJECT_NAME}_objects PRIVATE include)

add_executable(${PROJECT_NAME} src/main.c)
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objects)
