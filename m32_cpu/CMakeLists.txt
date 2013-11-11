cmake_minimum_required(VERSION 2.8)
project(m32_cpu)

set(CPP "main.cpp" "m32_cpu.cpp")
set(HPP "m32_cpu.h")

add_executable(${PROJECT_NAME} ${CPP} ${HPP})

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

include(CPack)
