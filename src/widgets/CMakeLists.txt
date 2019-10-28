find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

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

set(KIWI_WIDGETS_PUBLIC_INCLUDE_FILES
  ../../include/widgets/button.h
  ../../include/widgets/checkbox.h
  ../../include/widgets/editbox.h
  ../../include/widgets/frame.h
  ../../include/widgets/label.h
  ../../include/widgets/scrollbox.h
  ../../include/widgets/toggle.h
)

add_library(widgets
  ${KIWI_WIDGETS_SOURCE_FILES}
  ${KIWI_WIDGETS_PUBLIC_INCLUDE_FILES}
  ${KIWI_WIDGETS_PRIVATE_INCLUDE_FILES}
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
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
  $<INSTALL_INTERFACE:include/kiwi>
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

if (NOT MSVC)
  target_link_libraries(widgets PUBLIC "-lm")
endif()


