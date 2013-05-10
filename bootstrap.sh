#!/bin/sh
set -e

cmake_common="./buildtools/CMakeLists.common"
cmake_target="./CMakeLists.txt"

ln -fs "$cmake_common" "$cmake_target"

exit 0
