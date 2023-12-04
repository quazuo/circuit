cmake_minimum_required(VERSION 3.16)
project(circuit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -Wunused -Wcast-align \
    -Wdouble-promotion -Wmissing-declarations -Wmissing-include-dirs        \
    -Wnon-virtual-dtor -Wredundant-decls -Wodr -Wunreachable-code -Wshadow")

find_package(OpenGL REQUIRED)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW
        ${CMAKE_DL_LIBS}
)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

file(GLOB IMGUI_SRCS
        "deps/imgui/*.h"
        "deps/imgui/*.cpp"
)

set(IMGUI_IMPL_SRCS
        deps/imgui/backends/imgui_impl_glfw.h
        deps/imgui/backends/imgui_impl_glfw.cpp
        deps/imgui/backends/imgui_impl_opengl3.h
        deps/imgui/backends/imgui_impl_opengl3.cpp
)

set_source_files_properties(
        ${IMGUI_SRCS} ${IMGUI_IMPL_SRCS}
        PROPERTIES COMPILE_FLAGS "-w"
)

file(GLOB CIRCUIT_SRCS
        "src/*"
        "src/gui/*"
        "src/circuit/*"
        "src/circuit/boolean/*"
        "src/circuit/num/*"
)

add_executable(circuit ${CIRCUIT_SRCS} ${IMGUI_SRCS} ${IMGUI_IMPL_SRCS})
target_link_libraries(circuit ${ALL_LIBS})
