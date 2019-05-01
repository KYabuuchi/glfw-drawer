cmake_minimum_required(VERSION 3.0)

# Set default build type to Release
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Check C++17
include(CheckCXXCompilerFlag)
message("Using C++17")
enable_language(CXX)
check_cxx_compiler_flag("-std=gnu++17" COMPILER_SUPPORTS_CXX17)
if(NOT ${COMPILER_SUPPORTS_CXX17})
    message(
        FATAL_ERROR
            "${CMAKE_CXX_COMPILER} はC++17をサポートしてません。C++17に対応したコンパイラを指定してください。\n"
        )
endif()
set(CMAKE_CXX_STANDARD 17)
message("Compiler:\n\t${CMAKE_CXX_COMPILER}")

# Set warning flags
set(CXX_WARNING_FLAGS
    -Wall
    -Wextra
    -Wconversion
    -Wswitch-default
    -Wdisabled-optimization
    -Wformat
    -Winit-self
    -Woverloaded-virtual
    -Wfloat-equal
    -Wno-old-style-cast
    -Wno-pragmas)
foreach(FLAG IN LISTS CXX_WARNING_FLAGS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
endforeach()
message("Build type:\n\t${CMAKE_BUILD_TYPE}")

# Eigen3
find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIRS})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# header & source
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
file(GLOB SOURCES src/*/*.cpp)
message(${CMAKE_CURRENT_LIST_DIR}/include)

# library
add_library(glfw-drawer STATIC ${SOURCES})
target_link_libraries(glfw-drawer GL glfw GLEW pthread)

# If toplevel
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)
    set(TOPLEVEL True)
endif()

if(TOPLEVEL)
    add_executable(main main.cpp)
    target_link_libraries(main glfw-drawer)
endif()
