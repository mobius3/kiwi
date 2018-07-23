project(frame-family)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_STANDARD 99)
set(TILESETS "tileset.png;tileset-alloy.png;tileset-futterpedia.png;tileset-flat.png")

add_executable(frame-family frame-family.c ${TILESETS} Fontin-Regular.ttf)
target_link_libraries(frame-family KiWi ${SDL2_COMPOUND_LIBRARIES})

foreach(TILESET ${TILESETS})
add_custom_command(OUTPUT ${TILESET}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/${TILESET} ${CMAKE_CURRENT_BINARY_DIR}/${TILESET}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/${TILESET}
)
endforeach(TILESET ${TILESETS})

add_custom_command(OUTPUT Fontin-Regular.ttf
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/Fontin-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/Fontin-Regular.ttf
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/Fontin-Regular.ttf
)
  
