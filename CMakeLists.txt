# Date: 2013-06-01
# Author: Karsten Ahnert (karsten.ahnert@gmx.de)

project ( PROJECT )
cmake_minimum_required ( VERSION 2.6 )

# Add path for custom modules
set( CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
  )

find_package ( Boost REQUIRED )

set ( CMAKE_CXX_COMPILER "g++-4.8" )        ## Optimize
set ( CMAKE_CXX_FLAGS "-std=c++11" )        ## Optimize


include_directories ( ${Boost_INCLUDE_DIRS} )

find_library ( ZMQ_LIBRARY NAMES zmq PATHS /usr/lib /usr/local/lib )

add_executable ( sent_receive_server sent_receive_server.cpp )
target_link_libraries ( sent_receive_server ${ZMQ_LIBRARY} )

add_executable ( sent_receive_client sent_receive_client.cpp )
target_link_libraries ( sent_receive_client ${ZMQ_LIBRARY} )

add_executable( ipc ipc.cpp )
target_link_libraries ( ipc ${ZMQ_LIBRARY} )