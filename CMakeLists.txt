cmake_minimum_required(VERSION 3.16)
project(Qt_Oscilloscope)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets SerialPort REQUIRED)

add_executable(Oscilloscope
    src/main.cpp
    src/MainWindow.cpp
    src/OscilloscopeView.cpp
    src/SerialReader.cpp
)

target_include_directories(Oscilloscope PRIVATE include)

target_link_libraries(Oscilloscope Qt6::Widgets Qt6::SerialPort)

