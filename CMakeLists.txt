cmake_minimum_required(VERSION 3.16)


project (uasync)


add_library(uasync INTERFACE)
target_include_directories(uasync
    INTERFACE
        include
)
target_sources(uasync
    INTERFACE
        src/tcp.c
        src/udp.c
)

add_subdirectory(samples)
