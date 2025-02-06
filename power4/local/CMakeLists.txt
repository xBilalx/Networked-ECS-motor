cmake_minimum_required(VERSION 3.15)
project(power4_local)

add_executable(power4_local test.cpp)
target_link_libraries(power4_local PRIVATE ecs_common sfml-graphics sfml-window sfml-system)
