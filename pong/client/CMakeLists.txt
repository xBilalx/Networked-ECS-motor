cmake_minimum_required(VERSION 3.15)
project(pong_client)

add_executable(pong_client test.cpp)
target_link_libraries(pong_client PRIVATE ecs_common sfml-graphics sfml-window sfml-system)
