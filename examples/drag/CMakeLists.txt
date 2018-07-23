project(drag)
cmake_minimum_required(VERSION 2.8)

set(TILESETS "tileset.png;tileset-alloy.png;tileset-futterpedia.png;tileset-flat.png")
set(FONTS "Fontin-Regular.ttf;DejaVuSans.ttf;SourceSansPro-Semibold.ttf")

foreach(TILESET ${TILESETS})
  add_custom_command(OUTPUT ${TILESET}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/${TILESET} ${CMAKE_CURRENT_BINARY_DIR}/${TILESET}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/${TILESET}
  )
endforeach(TILESET ${TILESETS})

foreach(FONT ${FONTS})
  add_custom_command(OUTPUT ${FONT}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/${FONT} ${CMAKE_CURRENT_BINARY_DIR}/${FONT}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/${FONT}
  )
endforeach(FONT ${FONTS})

add_executable(drag drag.c ${FONTS} ${TILESETS})
target_link_libraries(drag KiWi ${SDL2_COMPOUND_LIBRARIES})
