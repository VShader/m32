cmake_minimum_required(VERSION 2.8)
project(m32_cpu)

set(CPP "main.cpp" "m32_cpu.cpp")
set(HPP "m32_cpu.h")

add_executable(${PROJECT_NAME} ${CPP} ${HPP})