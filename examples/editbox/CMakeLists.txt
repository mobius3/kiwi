project(editbox)
cmake_minimum_required(VERSION 2.8)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(editbox editbox.c tileset.png Fontin-Regular.ttf DejaVuSans.ttf)
target_link_libraries(editbox KiWi ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

add_custom_command(OUTPUT tileset.png
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/tileset.png ${CMAKE_CURRENT_BINARY_DIR}/tileset.png
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../tileset/tileset.png
)

add_custom_command(OUTPUT Fontin-Regular.ttf
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/Fontin-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/Fontin-Regular.ttf
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/Fontin-Regular.ttf
)
  
add_custom_command(OUTPUT DejaVuSans.ttf
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/DejaVuSans.ttf ${CMAKE_CURRENT_BINARY_DIR}/DejaVuSans.ttf
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../fonts/DejaVuSans.ttf
)  
