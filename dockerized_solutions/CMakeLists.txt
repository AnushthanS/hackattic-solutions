cmake_minimum_required(VERSION 4.2)

project(hackattic-dockerized-solutions VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/api/json-get.cpp
    src/api/trigger-push.cpp
    src/api/registry.cpp
    src/api/docker.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ./include/
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

# FetchContent
include(FetchContent)

FetchContent_Declare(
    glaze 
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git 
    GIT_TAG main 
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    cpr 
    GIT_REPOSITORY https://github.com/libcpr/cpr.git 
    GIT_TAG 1.14.2 
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze cpr)

target_link_libraries(${PROJECT_NAME} PRIVATE glaze::glaze cpr::cpr)
