cmake_minimum_required(VERSION 2.8.9)

project(m32_ide)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../m32_cpu)
set(qt_Header "ide_main_window.h" "code_editor.h")
set(qt_Sources "main.cpp" "ide_main_window.cpp" "code_editor.cpp")

qt5_wrap_cpp(qt_Header ${qt_Header})
qt5_wrap_cpp(qt_Sources ${qt_Sources})


add_executable(${PROJECT_NAME} ${qt_Sources} ${qt_Header})
# Use the Widgets module from Qt 5.
qt5_use_modules(${PROJECT_NAME} Widgets)

#install(TARGES ${PROJECT_NAME} DESTINATION bin)
include(CPack)
