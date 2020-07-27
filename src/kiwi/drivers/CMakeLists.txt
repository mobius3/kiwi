option(BUILD_DRIVERS_SDL2 "Build the SDL2 drivers for rendering and input" ON)

if (BUILD_DRIVERS_SDL2)
  add_subdirectory(sdl2)
endif()