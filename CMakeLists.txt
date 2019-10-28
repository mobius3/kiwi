cmake_minimum_required(VERSION 3.0)
project(kiwi LANGUAGES C VERSION 1.0.0)

# Point to our own cmake modules
set(KIWI_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "")
list(APPEND CMAKE_MODULE_PATH ${KIWI_ROOT}/cmake-modules)

if(WIN32)
	set(SDL2_ROOT "/sdl2" CACHE STRING "Root of the SDL2 installation")
endif(WIN32)

add_subdirectory(src)

include(cmake/setup-exports.cmake)
