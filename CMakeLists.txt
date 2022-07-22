cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(imagen)

add_subdirectory("${CMAKE_SOURCE_DIR}/dep/glfw")

add_library(base
  ${CMAKE_CURRENT_SOURCE_DIR}/base/base.mm
  ${CMAKE_CURRENT_SOURCE_DIR}/base/base.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/base)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dep/glm)

if(APPLE)
  target_link_libraries(base PUBLIC
    glfw
    "-framework Metal"
    "-framework MetalKit"
    "-framework AppKit"
    "-framework Foundation"
    "-framework QuartzCore")
endif()

link_libraries(base)

add_executable(lesson0 ${CMAKE_CURRENT_SOURCE_DIR}/lesson0/lesson0.cpp)
add_executable(lesson1 ${CMAKE_CURRENT_SOURCE_DIR}/lesson1/lesson1.cpp)
add_executable(lesson2 ${CMAKE_CURRENT_SOURCE_DIR}/lesson2/lesson2.cpp)
add_executable(lesson3 ${CMAKE_CURRENT_SOURCE_DIR}/lesson3/lesson3.cpp)
add_executable(lesson4 ${CMAKE_CURRENT_SOURCE_DIR}/lesson4/lesson4.cpp)
add_executable(lesson5 ${CMAKE_CURRENT_SOURCE_DIR}/lesson5/lesson5.cpp)
