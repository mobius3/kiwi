# Find SDL2
find_package(SDL2 REQUIRED)

# Include relevat dirs
include_directories(${SDL2_INCLUDE_DIR} ${KIWI_INCLUDE_DIR})

# The playground example
add_subdirectory(playground)