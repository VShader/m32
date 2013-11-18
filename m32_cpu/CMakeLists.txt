cmake_minimum_required(VERSION 2.8)
project(m32_cpu)

set(libM32CPU ${CMAKE_CURRENT_SOURCE_DIR}/../libM32CPU)
include_directories(${libM32CPU})
link_directories(${libM32CPU})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} libM32CPU)