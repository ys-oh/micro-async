
add_library(uasync INTERFACE)
target_include_directories(uasync
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../include
)

target_sources(uasync
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../src/tcp.c
        ${CMAKE_CURRENT_LIST_DIR}/../src/udp.c
)

include (FetchContent)
FetchContent_Declare(
    libev
    GIT_REPOSITORY git@github.com:ys-oh/libev.git
    GIT_TAG        dev
)

if(NOT libev_POPULATED)
    FetchContent_Populate(libev)
    add_subdirectory(${libev_SOURCE_DIR})
    target_sources(uasync INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../src/default.c)
endif()

target_link_libraries(uasync INTERFACE ev)

