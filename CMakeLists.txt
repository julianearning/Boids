cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Boids)

set (CMAKE_CXX_FLAGS "-O2")

# with MS Visual C++ we must explicity switch on proper exception handling
if (MSVC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif (MSVC)


# all source files
set(SRC bird.cpp swarm.cpp main.cpp)

# default target (created with "make")
add_executable (boids ${SRC})

find_package(SFML COMPONENTS window graphics system REQUIRED)
FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
target_link_libraries(boids PUBLIC sfml-graphics ${Boost_LIBRARIES})