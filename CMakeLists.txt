cmake_minimum_required(VERSION 3.15)
project(ECSNetwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(ecs)
add_subdirectory(pong)
add_subdirectory(power4)
