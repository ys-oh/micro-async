cmake_minimum_required(VERSION 3.16)


project (uasync)


add_library(uasync INTERFACE)
target_include_directories(uasync INTERFACE include)

add_subdirectory(samples)
