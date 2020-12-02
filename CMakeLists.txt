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
#
# Following KDE CMake coding conventions:
# https://community.kde.org/Policies/CMake_Coding_Style

cmake_minimum_required( VERSION 3.10 )
project( libyui )

# Options usage:
#
#   mkdir build
#   cd build
#   cmake -Dbuild_doc=on -Dbuild_examples=off ..

option( build_examples "Build C++ -based libyui examples"       on  )
option( build_doc      "Build class documentation"              off )

add_subdirectory( src )

if ( build_examples )
  add_subdirectory( examples )
endif()

if ( build_doc )
  add_subdirectory( doc )
endif()

# TO DO:
# - install buildtools/* to the system
