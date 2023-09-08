cmake_minimum_required(VERSION 3.23)
project(3DEngine C)

set(CMAKE_C_STANDARD 17)

add_executable(3DEngine main.c)

target_link_libraries(3DEngine -lmingw32 -lalleg44)
