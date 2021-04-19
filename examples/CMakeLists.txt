cmake_minimum_required(VERSION 3.5.1)

add_executable(simple_travel  simple_travel_domain.cpp simple_travel_problem.cpp simple_travel.cpp)
target_link_libraries(simple_travel ${TFD_CPP_LIBRARY})
