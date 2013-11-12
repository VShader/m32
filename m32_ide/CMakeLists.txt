cmake_minimum_required(VERSION 2.8.9)

project(m32_ide)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

INCLUDE_DIRECTORIES(../m32_cpu)
set(qt_Sources "main.cpp" "ide_main_window.cpp" "code_editor.cpp")
set(Header "ide_main_window.h" "code_editor.h")


add_executable(${PROJECT_NAME} ${qt_Sources} ${Header})

# Use the Widgets module from Qt 5.
qt5_use_modules(${PROJECT_NAME} Widgets)
