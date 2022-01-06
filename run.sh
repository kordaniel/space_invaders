#!/usr/bin/env bash

_DBG_DIR="build/debug/bin"
_REL_DIR="build/release/bin"

if [ $# -eq 1 ] && [[ "$1" == r* ]]; then
    if [ ! -d "${_REL_DIR}" ]; then
        echo "Directory './${_REL_DIR}' missing. Aborting."
        exit 1
    fi
    cd "${_REL_DIR}"
else
    if [ ! -d "${_DBG_DIR}" ]; then
        echo "Directory './${_DBG_DIR}' missing. Aborting."
        exit 1
    fi
    cd "${_DBG_DIR}"
fi

if [[ "${@: -1}" == "-log" ]]; then
    ./SpaceInvaders -log
else
    ./SpaceInvaders
fi

