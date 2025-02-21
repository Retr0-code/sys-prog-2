cmake_minimum_required(VERSION 3.17.0)

project(utility VERSION 1.0)

set(DIR_SOURCE src)

add_executable(${PROJECT_NAME} ${DIR_SOURCE}/main.c)
