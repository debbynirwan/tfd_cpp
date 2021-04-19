cmake_minimum_required(VERSION 3.5.1)
project(tfd_cpp VERSION 0.1.0)

#---- Project Name Alias ----
set(TFD_CPP_LIBRARY ${PROJECT_NAME})

#---- Enable C++17 ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#---- Include boost to add logging ----
find_package(Boost COMPONENTS log REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#---- project configuration ----
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_EXAMPLES "Build implementation examples" ON)
option(BUILD_UNIT_TESTS "Build the unit tests" ON)

if(BUILD_UNIT_TESTS)
    find_package(GTest REQUIRED)
endif()

#---- Source Files ----
list(APPEND TFD_CPP_SOURCE_FILES
    tfd_cpp/planning_domain.cpp
    tfd_cpp/planning_problem.cpp
    tfd_cpp/tfd.cpp
)

if (BUILD_SHARED_LIBS)
    add_library(${TFD_CPP_LIBRARY} SHARED ${TFD_CPP_SOURCE_FILES})
else()
    add_library(${TFD_CPP_LIBRARY} STATIC ${TFD_CPP_SOURCE_FILES})
endif()

target_link_libraries(${TFD_CPP_LIBRARY} Boost::log)

#---- Include Directories ----
target_include_directories(${TFD_CPP_LIBRARY} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_UNIT_TESTS)
    add_subdirectory(tests)
endif()
