cmake_minimum_required(VERSION 3.0.0)
project(inspire_robot VERSION 0.1.0 LANGUAGES C CXX)

include_directories(./src)

find_package(yaml-cpp REQUIRED)
add_executable(inspire_robot src/main.cpp)
target_link_libraries(inspire_robot pthread ECanVci usb)
add_executable(test_ src/test.cpp src/Inspire.hpp src/ECanVci.h)
target_link_libraries(test_ pthread ECanVci usb yaml-cpp)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
