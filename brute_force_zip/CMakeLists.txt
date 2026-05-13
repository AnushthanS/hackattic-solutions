cmake_minimum_required(VERSION 4.2)
project(brute_force_zip VERSION 0.1 LANGUAGES CXX)

# --- Global Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
find_package(Threads REQUIRED)
find_package(libzip REQUIRED)

include(FetchContent)

# Glaze (JSON)
FetchContent_Declare(
    glaze 
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git 
    GIT_TAG main 
    GIT_SHALLOW TRUE
)

# CPR (HTTP Requests)
FetchContent_Declare(
    cpr 
    GIT_REPOSITORY https://github.com/libcpr/cpr.git 
    GIT_TAG 1.14.2 
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze cpr)

# --- Executable Definitions ---

# Core Application
add_executable(brute_force_zip 
    src/main.cpp 
    src/worker.cpp 
    src/password_generator.cpp
)

# API/Network Helper
add_executable(get_problem src/get_problem.cpp)

# Tests
add_executable(password_tests src/test.cpp src/password_generator.cpp)
add_executable(limit_tests test-lim.cpp src/password_generator.cpp)
add_executable(zip_test src/zip_test.cpp)

# --- Target Properties & Linking ---

# Define a list for targets that share the main dependency stack
set(MAIN_TARGETS brute_force_zip get_problem)

foreach(target IN LISTS MAIN_TARGETS)
    target_include_directories(${target} PRIVATE include)
    target_link_libraries(${target} PRIVATE 
        Threads::Threads 
        libzip::zip 
        glaze::glaze 
        cpr::cpr
    )
    target_compile_definitions(${target} PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")
endforeach()

# Specialized linking for standalone tests
target_include_directories(password_tests PRIVATE include)
target_include_directories(limit_tests PRIVATE include)

target_include_directories(zip_test PRIVATE include)
target_link_libraries(zip_test PRIVATE libzip::zip)