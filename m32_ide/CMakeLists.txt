cmake_minimum_required(VERSION 2.8.9)
project(m32_ide)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)

#include m32Config
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add libM32CPU
set(libM32CPU ${CMAKE_CURRENT_SOURCE_DIR}/../libM32CPU)
include_directories(${libM32CPU})
link_directories(${libM32CPU})

set(qt_Header "ide_main_window.h" "code_editor.h")
set(qt_Sources "main.cpp" "ide_main_window.cpp" "code_editor.cpp")


add_executable(${PROJECT_NAME} ${qt_Sources} ${qt_Header})
target_link_libraries(${PROJECT_NAME} libM32CPU)

# Use the Widgets module from Qt 5.
qt5_use_modules(${PROJECT_NAME} Widgets)


install(TARGETS ${PROJECT_NAME} DESTINATION bin)
if(WIN32)
    set(QT5_BIN_DIR "C:/Qt/Qt5.1.1/5.1.1/msvc2012_64/bin")
    install(FILES ${QT5_BIN_DIR}/Qt5Core.dll
                  ${QT5_BIN_DIR}/Qt5Gui.dll
                  ${QT5_BIN_DIR}/Qt5Widgets.dll
            DESTINATION bin)
endif(WIN32)

