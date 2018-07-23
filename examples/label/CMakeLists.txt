project(label)
cmake_minimum_required(VERSION 2.8)

# build a label exe
add_executable(label label.c tileset.png Fontin-Regular.ttf)
target_link_libraries(label KiWi ${SDL2_COMPOUND_LIBRARIES})

add_custom_command(OUTPUT tileset.png
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/tileset.png ${CMAKE_CURRENT_BINARY_DIR}/tileset.png
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/tileset.png
)

add_custom_command(OUTPUT Fontin-Regular.ttf
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/Fontin-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/Fontin-Regular.ttf
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/Fontin-Regular.ttf
)
  
