find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include(GNUInstallDirs)
include(GenerateExportHeader)

set(KIWI_CORE_SOURCE_FILES
  eventwatcher.c
  gui.c
  rect.c
  renderdriver.c
  renderdriver_sdl2.c
  tilerenderer.c
  utf8.c
  widget.c
  widget_eventhandlers.c
)

set(KIWI_CORE_PRIVATE_INCLUDE_FILES
  gui_internal.h
  widget_internal.h
)


set(I ../../../include/kiwi)

set(KIWI_CORE_PUBLIC_INCLUDE_FILES
  ${I}/core/bool.h
  ${I}/core/eventwatcher.h
  ${I}/core/gui.h
  ${I}/core/macros.h
  ${I}/core/rect.h
  ${I}/core/renderdriver.h
  ${I}/core/renderdriver_sdl2.h
  ${I}/core/tilerenderer.h
  ${I}/core/utf8.h
  ${I}/core/widget.h
  ${CMAKE_CURRENT_BINARY_DIR}/core-export.h
)

add_library(core
  ${KIWI_CORE_SOURCE_FILES}
  ${KIWI_CORE_PUBLIC_INCLUDE_FILES}
  ${KIWI_CORE_PRIVATE_INCLUDE_FILES}
  ${I}/core.h
  ${CMAKE_CURRENT_BINARY_DIR}/core-export.h
)

generate_export_header(
  core
  BASE_NAME KIWI_CORE
  EXPORT_FILE_NAME core-export.h
)

target_link_libraries(core PUBLIC SDL2 SDL2_image SDL2_ttf PRIVATE c-rez::fonts)

add_library(kiwi::core ALIAS core)

set_target_properties(core PROPERTIES
  C_STANDARD 99
  VERSION ${kiwi_VERSION}
  PUBLIC_HEADER "${KIWI_CORE_PUBLIC_INCLUDE_FILES}"
  OUTPUT_NAME "kiwi-core"
)

target_include_directories(core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../include>
  $<INSTALL_INTERFACE:include>
)

target_compile_options(core PRIVATE
  $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:GNU>>:-Wall -Wextra -pedantic -fvisibility=hidden -Werror>
  $<$<C_COMPILER_ID:MSVC>:/W3 /WX /wd4820 /wd4668 /wd4204>)

install(TARGETS core
  EXPORT kiwi-targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kiwi/core
)

install(FILES ${I}/core.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kiwi)

if (NOT MSVC)
  target_link_libraries(core PUBLIC "-lm")
endif()


