cmake_minimum_required(VERSION 2.8.3)
project(despatcher)

find_package(catkin REQUIRED)

# find_package(Boost REQUIRED COMPONENTS system)

catkin_package(
)

include_directories (include)

add_library(exek_despatcher src/exek_despatcher.cpp)

add_executable (despatcher src/despatcher.cpp)

target_link_libraries (despatcher
  exek_despatcher
  peiskernel_mt
  pthread
)