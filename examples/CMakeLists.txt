# Find SDL2
find_package(SDL2 REQUIRED)

# Include relevant dirs
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${KIWI_INCLUDE_DIR})

# Ask for C99
set(CMAKE_C_STANDARD 99)

# The label example
add_subdirectory(label)

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
