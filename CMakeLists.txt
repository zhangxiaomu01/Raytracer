cmake_minimum_required(VERSION 3.20)

project(Raytracer
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "Ray Tracing in One Weekend learning project"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(MSVC)
    string(APPEND CMAKE_CXX_FLAGS " /W4 /permissive- /utf-8")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " /Zi /Od")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " /O2")
else()
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wpedantic")
    string(APPEND CMAKE_CXX_FLAGS_DEBUG " -g -O0")
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -O3")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/objects)
include_directories(${CMAKE_SOURCE_DIR}/src/math)
include_directories(${CMAKE_SOURCE_DIR}/src/camera)
include_directories(${CMAKE_SOURCE_DIR}/src/utility)
include_directories(${CMAKE_SOURCE_DIR}/src/materials)
include_directories(${CMAKE_SOURCE_DIR}/src/texture)
include_directories(${CMAKE_SOURCE_DIR}/src/scene)
include_directories(${CMAKE_SOURCE_DIR}/src/thirdParty)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/camera/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/objects/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/materials/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/math/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/utility/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/texture/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/scene/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/thirdParty/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
