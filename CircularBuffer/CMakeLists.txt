cmake_minimum_required(VERSION 3.23)
project(
        lab8
        VERSION 1.0
        LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS  "-static")

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)