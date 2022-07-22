cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(imagen)

add_subdirectory("${CMAKE_SOURCE_DIR}/dep/glfw")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/base)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dep/glfw/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dep/glm)

# For Apple, we use Metal for the backend (OpenGL deprecation bruh)
if(APPLE)
  add_library(base
    ${CMAKE_CURRENT_SOURCE_DIR}/base/base.mm
    ${CMAKE_CURRENT_SOURCE_DIR}/base/base.hpp)

  target_link_libraries(base PUBLIC
    glfw
    "-framework Metal"
    "-framework MetalKit"
    "-framework AppKit"
    "-framework Foundation"
    "-framework QuartzCore")

  link_libraries(base)
# For Windows and Linux, we just use OpenGL + GLFW (plain and simple)
elseif(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_MBCS")

  add_library(base
    ${CMAKE_CURRENT_SOURCE_DIR}/base/base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base/base.hpp)

  target_link_libraries(base PUBLIC
    glfw GL user32.lib gdi32.lib winmm.lib msvcrt.lib)

  link_libraries(base)
else()
  add_library(base
    ${CMAKE_CURRENT_SOURCE_DIR}/base/base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/base/base.hpp)

  target_link_libraries(base PUBLIC
    glfw GL)

  link_libraries(base)
endif()


add_executable(lesson0 ${CMAKE_CURRENT_SOURCE_DIR}/lesson0/lesson0.cpp)
add_executable(lesson1 ${CMAKE_CURRENT_SOURCE_DIR}/lesson1/lesson1.cpp)
add_executable(lesson2 ${CMAKE_CURRENT_SOURCE_DIR}/lesson2/lesson2.cpp)
add_executable(lesson3 ${CMAKE_CURRENT_SOURCE_DIR}/lesson3/lesson3.cpp)
add_executable(lesson4 ${CMAKE_CURRENT_SOURCE_DIR}/lesson4/lesson4.cpp)
add_executable(lesson5 ${CMAKE_CURRENT_SOURCE_DIR}/lesson5/lesson5.cpp)
