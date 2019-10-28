set(SDL2_BUILDING_LIBRARY TRUE)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

option(KIWI_BUILD_SHARED "Build Kiwi as a static library" ON)

include(${KIWI_ROOT}/resources/kiwires.cmake)

set(API_HEADERS
  gui.h
  button.h
  editbox.h
  frame.h
  label.h
  macros.h
  tilerenderer.h
  widget.h
  scrollbox.h
  renderdriver_sdl2.h
  renderdriver.h
  rect.h
  bool.h
  toggle.h
  checkbox.h
)

set(LIB_SOURCES
  scrollbox_internal.c
  scrollbox.c
  editbox_internal.c
  utf8.c
  editbox.c
  eventwatcher.c
  button.c
  label.c
  label_internal.c
  gui.c
  frame.c
  frame_internal.c
  tilerenderer.c
  widget.c
  widget_eventhandlers.c
  renderdriver.c
  renderdriver_sdl2.c
  rect.c
  toggle.c
  checkbox.c
)

kiwires(HEADER resources.h BODY resources.c RESOURCES resources/sourcesans-pro-semibold.ttf)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
if(KIWI_BUILD_SHARED)
add_library(KiWi SHARED ${LIB_SOURCES} ${API_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/resources.c)
else()
add_library(KiWi STATIC ${LIB_SOURCES} ${API_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/resources.c)
endif()
target_link_libraries(KiWi ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
if(CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(KiWi PRIVATE -fvisibility=hidden -std=c89 -pedantic-errors -Wall -Wextra)
endif(CMAKE_COMPILER_IS_GNUCC)

# Configure install stuff
install(TARGETS KiWi EXPORT KiWiTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(FILES ${API_HEADERS} DESTINATION include/KiWi)
