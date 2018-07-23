# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

set (SDL2_COMPOUND_LIBRARIES ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

IF(MINGW)
	set(SDL2_COMPOUND_LIBRARIES ${MINGW32_LIBRARY} ${SDL2_COMPOUND_LIBRARIES})
ENDIF(MINGW)

# Include relevant dirs
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${KIWI_INCLUDE_DIR})

# Ask for C99
set(CMAKE_C_STANDARD 99)

# Custom render function example
add_subdirectory(custom-render)

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

# Debug gizmos example
add_subdirectory(debug-gizmos)
