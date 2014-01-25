cmake_minimum_required(VERSION 2.8)
project(libM32CPU)

set(CPP "m32_cpu.cpp")
set(HPP "m32_cpu.h" "m32_defines.h")

add_library(${PROJECT_NAME} STATIC ${CPP} ${HPP})
