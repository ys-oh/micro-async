
# tcp samples
add_executable(tcp_client tcp_client.c)
target_link_libraries(tcp_client PUBLIC uasync)

add_executable(tcp_server tcp_server.c)
target_link_libraries(tcp_server PUBLIC uasync)


# udp samples
add_executable(udp_client udp_client.c)
target_link_libraries(udp_client PUBLIC uasync)

add_executable(udp_server udp_server.c)
target_link_libraries(udp_server PUBLIC uasync)

