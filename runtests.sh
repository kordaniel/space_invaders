#!/usr/bin/env bash

_DBG_DIR="build/debug"
_TESTS=(
    "SizeTest"
    "InputTest"
)

if [ ! -d "${_DBG_DIR}/bin" ]; then
    echo "Directory './${_DBG_DIR}/bin' missing. Aborting."
    exit 1
fi

if [[ "$1" == c* ]]; then
    if ! command -v ctest &> /dev/null; then
        echo "'ctest' not found. Running the individual tests instead."
    else
        cd "${_DBG_DIR}"
        ctest
    fi
fi

for test in "${_TESTS[@]}"; do
    if [ ! -x "${_DBG_DIR}/bin/${test}" ]; then
        echo "Test $test not found. Skipping.."
        continue
    fi

    echo "Running test './${_DBG_DIR}/bin/${test}':"
    ./"${_DBG_DIR}/bin/${test}"
done

