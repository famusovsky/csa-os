project(os-phw3-45p C)

cmake_minimum_required(VERSION 3.10)

add_library( FUNCS_FOR_TCP
    funcs_for_tcp.c
    funcs_for_tcp.h
)

add_executable(hotel
    hotel.c
)

add_executable(visitors
    visitors.c
)

add_executable(street
    street.c
)

target_link_libraries(hotel FUNCS_FOR_TCP)
target_link_libraries(visitors FUNCS_FOR_TCP)
target_link_libraries(street FUNCS_FOR_TCP)