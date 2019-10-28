find_package(SDL2 REQUIRED)

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

set(KIWI_CORE_PUBLIC_INCLUDE_FILES
  ../../include/core/bool.h
  ../../include/core/eventwatcher.h
  ../../include/core/gui.h
  ../../include/core/macros.h
  ../../include/core/rect.h
  ../../include/core/renderdriver.h
  ../../include/core/renderdriver_sdl2.h
  ../../include/core/tilerenderer.h
  ../../include/core/utf8.h
  ../../include/core/widget.h
)

add_library(core
  ${KIWI_CORE_SOURCE_FILES}
  ${KIWI_CORE_PUBLIC_INCLUDE_FILES}
  ${KIWI_CORE_PRIVATE_INCLUDE_FILES}
)

target_link_libraries(core PUBLIC SDL2)

add_library(kiwi::core ALIAS core)

set_target_properties(core PROPERTIES
  C_STANDARD 99
  VERSION ${kiwi_VERSION}
  PUBLIC_HEADER "${KIWI_CORE_PUBLIC_INCLUDE_FILES}"
  OUTPUT_NAME "kiwi-core"
)

target_include_directories(core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
  $<INSTALL_INTERFACE:include/kiwi>
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

if (NOT MSVC)
  target_link_libraries(core PUBLIC "-lm")
endif()


