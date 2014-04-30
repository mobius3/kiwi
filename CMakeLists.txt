project(sdl_gui)
cmake_minimum_required(VERSION 2.8)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

# Define this as top-level
set(KIWI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Add global definitions
add_definitions("-Wall")

# Add Debug definitions
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_DEBUG} -DNDEBUG")

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples)