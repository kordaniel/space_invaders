cmake_minimum_required(VERSION 3.16)

add_compile_options("${CXX_FLAGS}" "$<$<CONFIG:Debug>:${CXX_FLAGS_DEBUG}>")
add_link_options("$<$<CONFIG:Debug>:${CXX_LDFLAGS_DEBUG}>")

include_directories(
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
    PRIVATE "${CMAKE_SOURCE_DIR}/src"
)
link_libraries(gtest_main)

set(SizeTest "SizeTest")
set(SizeTestSources
    "SizeTest.cpp"
    "${CMAKE_SOURCE_DIR}/src/Size.cpp"
)

add_executable("${SizeTest}" "${SizeTestSources}")
add_test(
    NAME "${SizeTest}"
    COMMAND "${SizeTest}"
)

set(InputTest "InputTest")
set(InputTestSources
    "InputTest.cpp"
    "${CMAKE_SOURCE_DIR}/src/Input.cpp"
)

add_executable("${InputTest}" "${InputTestSources}")
target_link_libraries("${InputTest}" PRIVATE glfw)
add_test(
    NAME "${InputTest}"
    COMMAND "${InputTest}"
)
