cmake_minimum_required(VERSION 3.15)
project(ecs_common)

add_library(ecs_common INTERFACE)
target_include_directories(ecs_common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/common/ecs)
