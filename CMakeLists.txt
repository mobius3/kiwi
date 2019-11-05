cmake_minimum_required(VERSION 3.0)
project(kiwi LANGUAGES C VERSION 1.0.0)

# Respect VISIBILITY_PRESET
cmake_policy(SET CMP0063 NEW)

# new behavior for <package>_ROOT variable handling
cmake_policy(SET CMP0074 NEW)

# Point to our own cmake modules
set(KIWI_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "")
list(APPEND CMAKE_MODULE_PATH ${KIWI_ROOT}/cmake-modules)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

set(BUILD_SHARED_LIBS ON)

if(WIN32)
	set(SDL2_ROOT "/sdl2" CACHE STRING "Root of the SDL2 installation")
endif(WIN32)

add_subdirectory(third-party EXCLUDE_FROM_ALL)
add_subdirectory(assets)
add_subdirectory(src)

include(cmake/setup-exports.cmake)
include(cmake/kiwi-cpack.cmake)
