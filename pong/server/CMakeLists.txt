cmake_minimum_required(VERSION 3.15)
project(pong_server)

add_executable(pong_server test.cpp)
target_link_libraries(pong_server PRIVATE ecs_common sfml-graphics sfml-window sfml-system)
