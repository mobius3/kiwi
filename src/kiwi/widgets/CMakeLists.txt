find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include(GNUInstallDirs)
include(GenerateExportHeader)

set(KIWI_WIDGETS_SOURCE_FILES
  button.c
  checkbox.c
  editbox.c
  editbox_internal.c
  frame.c
  frame_internal.c
  label.c
  label_internal.c
  scrollbox.c
  scrollbox_internal.c
  toggle.c
)

set(KIWI_WIDGETS_PRIVATE_INCLUDE_FILES
  button_internal.h
  editbox_internal.h
  frame_internal.h
  label_internal.h
  scrollbox_internal.h
)

set(I ../../../include/kiwi)

set(KIWI_WIDGETS_PUBLIC_INCLUDE_FILES
  ${I}/widgets/button.h
  ${I}/widgets/checkbox.h
  ${I}/widgets/editbox.h
  ${I}/widgets/frame.h
  ${I}/widgets/label.h
  ${I}/widgets/scrollbox.h
  ${I}/widgets/toggle.h
  ${CMAKE_CURRENT_BINARY_DIR}/widgets-export.h
)

add_library(widgets
  ${KIWI_WIDGETS_SOURCE_FILES}
  ${KIWI_WIDGETS_PUBLIC_INCLUDE_FILES}
  ${KIWI_WIDGETS_PRIVATE_INCLUDE_FILES}
  ${I}/widgets.h
  ${CMAKE_CURRENT_BINARY_DIR}/widgets-export.h
)

# Tell MSVC we know sprintf is unsafe
if (WIN32)
  target_compile_definitions(widgets PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

generate_export_header(
  core
  BASE_NAME KIWI_WIDGETS
  EXPORT_FILE_NAME widgets-export.h
)

target_link_libraries(widgets PUBLIC kiwi::core SDL2 SDL2_image SDL2_ttf)

add_library(kiwi::widgets ALIAS widgets)

set_target_properties(widgets PROPERTIES
  C_STANDARD 99
  VERSION ${kiwi_VERSION}
  PUBLIC_HEADER "${KIWI_WIDGETS_PUBLIC_INCLUDE_FILES}"
  OUTPUT_NAME "kiwi-widgets"
)

target_include_directories(widgets PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../include>
  $<INSTALL_INTERFACE:include>
)

target_compile_options(widgets PRIVATE
  $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:GNU>>:-Wall -Wextra -pedantic -fvisibility=hidden -Werror>
  $<$<C_COMPILER_ID:MSVC>:/W3 /WX /wd4820 /wd4668 /wd4204>)

install(TARGETS widgets
  EXPORT kiwi-targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kiwi/widgets
)

install(FILES ${I}/widgets.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kiwi)

if (NOT MSVC)
  target_link_libraries(widgets PUBLIC "-lm")
endif()


