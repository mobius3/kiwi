# Find SDL2
find_package(SDL2 REQUIRED)

# Include relevat dirs
include_directories(${SDL2_INCLUDE_DIR} ${KIWI_INCLUDE_DIR})

# The playground example
add_subdirectory(playground)

# Frame family example
add_subdirectory(frame-family)

# Editbox example
add_subdirectory(editbox)

# Styleswitcher example
add_subdirectory(styleswitcher)

# Scroll area example
add_subdirectory(scrollbox)

# Drag widget example
add_subdirectory(drag)
