cmake_minimum_required(VERSION 2.6)
project(DELVEWORKS)

include_directories(
src
)


add_executable(demo
src/demo/main.cpp
)

target_link_libraries(demo Engine)


add_library(Engine SHARED
src/Engine/Core.cpp
src/Engine/Core.h
)