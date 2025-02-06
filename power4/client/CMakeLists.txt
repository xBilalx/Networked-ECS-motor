cmake_minimum_required(VERSION 3.15)
project(power4_client)

add_executable(power4_client test.cpp)
target_link_libraries(power4_client PRIVATE ecs_common sfml-graphics sfml-window sfml-system)
