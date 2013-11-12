# Copyright: (C) 2011 EFAA Consortium
# Authors: Uriel Martinez
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

set(PROJECTNAME testEdgeFollowing)
project(${PROJECTNAME})

find_package(YARP)
find_package(ICUB)

set(CMAKE_CXX_FLAGS "-I$ENV{YARP_ROOT}/src/modules/serial -Wall -lstdc++ -lusb -I$ENV{NXTPP_PATH}/include -lnxtpp")

file(GLOB folder_source src/*.cpp)
file(GLOB folder_header include/*.h)
source_group("Source Files" FILES ${folder_source})
source_group("Header Files" FILES ${folder_header})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ICUB_INCLUDE_DIRS} ${YARP_INCLUDE_DIRS})
add_executable(${PROJECTNAME} ${folder_source} ${folder_header})
target_link_libraries(${PROJECTNAME} icubmod yarpmod ${CMAKE_CXX_FLAGS} ${YARP_LIBRARIES})
install(TARGETS ${PROJECTNAME} DESTINATION bin)
