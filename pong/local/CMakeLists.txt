cmake_minimum_required(VERSION 3.15)
project(pong_local)

add_executable(pong_local test.cpp)
target_link_libraries(pong_local PRIVATE ecs_common sfml-graphics sfml-window sfml-system)
