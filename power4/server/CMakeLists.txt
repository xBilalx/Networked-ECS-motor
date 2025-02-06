cmake_minimum_required(VERSION 3.15)
project(power4_server)

add_executable(power4_server test.cpp)
target_link_libraries(power4_server PRIVATE ecs_common sfml-graphics sfml-window sfml-system)
