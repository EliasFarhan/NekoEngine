
add_executable(udp_client client/main.cpp)
add_executable(udp_server server/main.cpp)

target_link_libraries(udp_client sfml-network)
target_link_libraries(udp_server sfml-network)