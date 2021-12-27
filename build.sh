#!/usr/bin/env bash

# Add the following flag to the cmake commands to specify which compiler to use
#-D CMAKE_CXX_COMPILER=<compiler-bin> #e.g.to use GCC on macos: g++-11

_DBG_DIR="build/debug"
_REL_DIR="build/release"

if [ $# -eq 1 ] && [[ "$1" == rel* ]]; then
    if [ ! -d "${_REL_DIR}" ]; then
        echo "Directory './${_REL_DIR}' missing. Aborting."
        echo "Run './configure.sh release' first to set up the build environment."
        exit 1
    fi
    echo "Building RELEASE version"
    cd "${_REL_DIR}"
    make
else
    if [ ! -d "${_DBG_DIR}" ]; then
        echo "Directory './${_DBG_DIR}' missing. Aborting."
        echo "Run './configure.sh' first to set up the build environment for debugging."
        exit 1
    fi
    echo "Building DEBUG version"
    cd "${_DBG_DIR}"
    make VERBOSE=1
fi

