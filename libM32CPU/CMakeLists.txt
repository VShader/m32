cmake_minimum_required(VERSION 2.8)
project(libM32CPU)

set(CPP "m32_cpu.cpp")
set(HPP "m32_cpu.h")

add_library(${PROJECT_NAME} STATIC ${CPP} ${HPP})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
#install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

#include(CPack)
