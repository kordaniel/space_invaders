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

if [ "$#" -eq 1 ] && [[ "$1" == c* ]]; then
    if ! command -v ctest &> /dev/null; then
        echo "'ctest' not found. Running the individual tests instead."
    else
        cd "${_DBG_DIR}"
        ctest
        exit # Exit with the exit code returned by ctest
    fi
elif [ "$#" -gt 0 ]; then
    echo "Bad argument(s). Run this script without any arguments or with the single argument 'ctest'"
    exit 2
fi

exit_status=0

for test in "${_TESTS[@]}"; do
    if [ ! -x "${_DBG_DIR}/bin/${test}" ]; then
        echo "Test ${test} not found. Skipping.."
        exit_status=2
        continue
    fi

    echo "Running test './${_DBG_DIR}/bin/${test}':"
    ./"${_DBG_DIR}/bin/${test}"
    test_exit_status="$?"
    test "$test_exit_status" -ne 0 && exit_status="${test_exit_status}"
done

exit $exit_status
