cmake_minimum_required(VERSION 3.5)
project(MCM2017)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(SOURCE_FILES src/main.cpp src/Layout.cpp src/Car.cpp src/HumanCar.cpp src/AutoCar.cpp)
add_executable(MCM2017 ${SOURCE_FILES})