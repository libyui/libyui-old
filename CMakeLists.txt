# CMakeLists.txt for libyui
#
# Usage:
#
#   mkdir build
#   cd build
#   cmake ..
#
#   make
#   sudo make install
#
# Restart with a clean build environment:
#
#   rm -rf build

cmake_minimum_required( VERSION 3.10 )
project( libyui )

# Options usage:
#
#   cmake -DBUILD_DOC=on -DBUILD_EXAMPLES=off ..

option( BUILD_EXAMPLES  "Build C++ -based libyui examples"      on  )
option( BUILD_DOC       "Build class documentation"             off )
option( WERROR          "Treat all compiler warnings as errors" on  )


# Initialize compiler flags

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
if ( WERROR )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
endif()
message( "Using CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )


#
# Descend into subdirectories
#

add_subdirectory( src )

if ( BUILD_EXAMPLES )
  add_subdirectory( examples )
endif()

if ( BUILD_DOC )
  add_subdirectory( doc )
endif()

# TO DO:
# - install buildtools/* to the system
