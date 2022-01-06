#!/usr/bin/env bash

# Add the following flag to the cmake commands to specify which compiler to use
#-D CMAKE_CXX_COMPILER=<compiler-bin> #e.g.to use GCC on macos: g++-11

_DBG_DIR="build/debug"
_REL_DIR="build/release"

if [ $# -eq 1 ] && [[ "$1" == r* ]]; then
    echo "Configuring RELEASE mode"
    cmake -D CMAKE_BUILD_TYPE=Release -S . -B ${_REL_DIR}
else
    echo "Configuring DEBUG mode"
    cmake -D CMAKE_BUILD_TYPE=Debug -S . -B ${_DBG_DIR}
fi

