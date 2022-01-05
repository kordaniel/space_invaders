#!/usr/bin/env bash

if [ $# -eq 0 ]; then
    echo "Cleaning up debug build"
    rm -r build/debug
elif [[ "$1" == a* ]]; then
    echo "Cleaning up everything"
    rm -r build
elif [[ "$1" == r* ]]; then
    echo "Cleaning up release build"
    rm -r build/release
else
    [[ "$1" != h* ]] && echo -n "Bad argument. "
    echo "Usage: ${0} <arg>"
    echo "Supported arguments:"
    echo -e "\thelp    - print this help message"
    echo -e "\tnothing - DEFAULT. Passing no args cleans up debug buildfiles"
    echo -e "\trelease - Cleans up release buildfiles"
    echo -e "\tall     - Cleans up everything"
fi

