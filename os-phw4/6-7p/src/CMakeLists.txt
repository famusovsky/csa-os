cmake_minimum_required(VERSION 3.10)

project(os-phw4-45p C)

add_library( FUNCS_FOR_UDP
    funcs_for_udp.c
    funcs_for_udp.h
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

add_executable(display
    display
)

target_link_libraries(hotel FUNCS_FOR_UDP)
target_link_libraries(visitors FUNCS_FOR_UDP)
target_link_libraries(street FUNCS_FOR_UDP)
target_link_libraries(display FUNCS_FOR_UDP)