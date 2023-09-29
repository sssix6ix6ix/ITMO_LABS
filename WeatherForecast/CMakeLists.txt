cmake_minimum_required(VERSION 3.23)
project(labwork_10_sssix6ix6ix)

set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR})
endif()

include_directories(.)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(lib)
add_subdirectory(bin)
#
#include(FetchContent)
#FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
#        GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
#FetchContent_MakeAvailable(cpr)
#
#FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json.git
#        GIT_TAG v3.10.4)
#FetchContent_MakeAvailable(nlohmann_json)
#
#target_link_libraries(labwork_10_sssix6ix6ix PRIVATE cpr::cpr)
#target_link_libraries(labwork_10_sssix6ix6ix PRIVATE nlohmann_json::nlohmann_json)
