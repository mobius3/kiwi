find_package(SDL2 REQUIRED)
find_package(font-chef REQUIRED)
include(GNUInstallDirs)
include(GenerateExportHeader)

set(KIWI_CORE_PUBLIC_INCLUDE_FILES
  ${I}/drivers/sdl2/sdl2-driver.h
  ${CMAKE_CURRENT_BINARY_DIR}/kiwi/drivers/sdl2/sdl2-driver-export.h
  )

add_library(kiwi-sdl2
  sdl2-driver.c
  sdl2-font.c
  sdl2-memory.c
  sdl2-texture.c
)

add_library(kiwi::sdl2 ALIAS kiwi-sdl2)

set_target_properties(kiwi-sdl2 PROPERTIES
  C_STANDARD 99
  VERSION ${kiwi_VERSION}
  SOVERSION ${kiwi_VERSION_MAJOR}
  PUBLIC_HEADER "${KIWI_CORE_PUBLIC_INCLUDE_FILES}"
  OUTPUT_NAME "kiwi-sdl2"
)

target_include_directories(kiwi-sdl2 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../../include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(kiwi-sdl2 PUBLIC kiwi::core font-chef SDL2)

generate_export_header(
  kiwi-sdl2
  BASE_NAME KIWI_DRIVERS_SDL2
  EXPORT_FILE_NAME kiwi/drivers/sdl2/sdl2-driver-export.h
)