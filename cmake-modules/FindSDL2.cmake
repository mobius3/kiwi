# This file is mostly based around the old FindSDL.cmake that is included in
# CMake's installation. This will attempt to find SDL2 library and include
# directories installed in your system and variables below accordingly. With
# the latest additions, it will also define targets to be used in
# target_link_libraries.
#
# This module defines
# TARGET SDL2, so you can target_link_libraries(YourLib SDL2)
# TARGET SDL2main. You don't need to use this explicity, it is automatically
#                  added as a dependency.
# SDL2_LIBRARY, a list of link flags, including SDL
# SDL2_FOUND, if false, do not try to link to SDL2
# SDL2_INCLUDE_DIR, where to find SDL.h
#
# This module responds to the the flag:
# SDL2_BUILDING_LIBRARY
# If this is defined, then no SDL2main will be linked in because
# only applications need main().
# Otherwise, it is assumed you are building an application and this
# module will attempt to locate and set the the proper link flags
# as part of the returned SDL2_LIBRARY variable.
#
# Don't forget to include SDLmain.h and SDLmain.m your project for the
# OS X framework based version. (Other versions link to -lSDL2main which
# this module will try to find on your behalf.) Also for OS X, this
# module will automatically add the -framework Cocoa on your behalf.
#
#
# Additional Note: If you see an empty SDL2_LIBRARY_TEMP in your configuration
# and no SDL2_LIBRARY, it means CMake did not find your SDL2 library
# (SDL2.dll, libsdl2.so, SDL2.framework, etc).
# Set SDL2_LIBRARY_TEMP to point to your SDL2 library, and configure again.
# Similarly, if you see an empty SDL2MAIN_LIBRARY, you should set this value
# as appropriate. These values are used to generate the final SDL2_LIBRARY
# variable, but when these values are unset, SDL2_LIBRARY does not get created.
#
#
# $SDL2DIR is an environment variable that would
# correspond to the ./configure --prefix=$SDL2DIR
# used in building SDL2.
# l.e.galup  9-20-02
#
# Modified by Eric Wing.
# Added code to assist with automated building by using environmental variables
# and providing a more controlled/consistent search behavior.
# Added new modifications to recognize OS X frameworks and
# additional Unix paths (FreeBSD, etc).
# Also corrected the header search path to follow "proper" SDL guidelines.
# Added a search for SDL2main which is needed by some platforms.
# Added a search for threads which is needed by some platforms.
# Added needed compile switches for MinGW.
#
# On OSX, this will prefer the Framework version (if found) over others.
# People will have to manually change the cache values of
# SDL2_LIBRARY to override this selection or set the CMake environment
# CMAKE_INCLUDE_PATH to modify the search paths.
#
# Note that the header path has changed from SDL2/SDL.h to just SDL.h
# This needed to change because "proper" SDL convention
# is #include "SDL.h", not <SDL2/SDL.h>. This is done for portability
# reasons because not all systems place things in SDL2/ (see FreeBSD).

#=============================================================================
# Copyright 2003-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

set(SDL2_SEARCH_PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  ${SDL2_ROOT}
)

find_path(SDL2_INCLUDE_DIR SDL.h
  HINTS
  $ENV{SDL2DIR}
  PATH_SUFFIXES include/SDL2 include
  PATHS ${SDL2_SEARCH_PATHS}
)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(PATH_SUFFIXES lib64 lib/x64 lib)
else ()
  set(PATH_SUFFIXES lib/x86 lib)
endif ()

find_library(SDL2_LIBRARY_LOCATION
  NAMES SDL2
  HINTS
  $ENV{SDL2DIR}
  PATH_SUFFIXES ${PATH_SUFFIXES}
  PATHS ${SDL2_SEARCH_PATHS}
)

set(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_LOCATION})

if (NOT SDL2_BUILDING_LIBRARY)
  if (NOT ${SDL2_INCLUDE_DIR} MATCHES ".framework")
    # Non-OS X framework versions expect you to also dynamically link to
    # SDL2main. This is mainly for Windows and OS X. Other (Unix) platforms
    # seem to provide SDL2main for compatibility even though they don't
    # necessarily need it.
    find_library(SDL2MAIN_LIBRARY
      NAMES SDL2main
      HINTS
      $ENV{SDL2DIR}
      PATH_SUFFIXES ${PATH_SUFFIXES}
      PATHS ${SDL2_SEARCH_PATHS}
    )
  endif()
endif()

# SDL2 may require threads on your system.
# The Apple build may not need an explicit flag because one of the
# frameworks may already provide it.
# But for non-OSX systems, I will use the CMake Threads package.
if (NOT APPLE)
  find_package(Threads)
endif ()

# MinGW needs an additional link flag, -mwindows
# It's total link flags should look like -lmingw32 -lSDL2main -lSDL2 -mwindows
if (MINGW)
  set(MINGW32_LIBRARY mingw32 "-mwindows" CACHE STRING "mwindows for MinGW")
endif ()

if (SDL2_LIBRARY_TEMP)
  # If SDL2 was found, add a library target.
  add_library(SDL2 SHARED IMPORTED)

  if (WIN32)
    find_file(SDL2_DLL
      SDL2.dll
      HINTS
      $ENV{SDL2DIR}
      PATH_SUFFIXES ${PATH_SUFFIXES}
      PATHS ${SDL2_SEARCH_PATHS}
    )
    set_target_properties(SDL2 PROPERTIES
      IMPORTED_LOCATION ${SDL2_DLL}
      IMPORTED_IMPLIB ${SDL2_LIBRARY_LOCATION}
      INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIR})
  else()
    set_target_properties(SDL2 PROPERTIES
      IMPORTED_LOCATION ${SDL2_LIBRARY_LOCATION}
      INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIR})
  endif(WIN32)

  # For SDL2main
  if (NOT SDL2_BUILDING_LIBRARY)
    if (SDL2MAIN_LIBRARY)
      # If the SDL2 main library is needed and found, add a target to it.
      # Also, makes the top-level SDL2 target depend on it.
      add_library(SDL2main STATIC IMPORTED)
      set_property(TARGET SDL2main PROPERTY IMPORTED_LOCATION ${SDL2MAIN_LIBRARY})
      target_link_libraries(SDL2 INTERFACE SDL2main)

      # Keep increasing SDL2_LIBRARY_TEMP
      set(SDL2_LIBRARY_TEMP ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP})
    endif ()
  endif ()

  # For OS X, SDL2 uses Cocoa as a backend so it must link to Cocoa.
  # CMake doesn't display the -framework Cocoa string in the UI even
  # though it actually is there if I modify a pre-used variable.
  # I think it has something to do with the CACHE STRING.
  # So I use a temporary variable until the end so I can set the
  # "real" variable in one-shot.
  if (APPLE)
    set(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} "-framework Cocoa")
    get_property(INTERFACE_LINK_LIBRARIES TARGET SDL2 PROPERTY INTERFACE_LINK_LIBRARIES)
    set_property(TARGET SDL2 PROPERTY
      INTERFACE_LINK_LIBRARIES ${INTERFACE_LINK_LIBRARIES} "-framework Cocoa")
  endif ()

  # For threads, as mentioned Apple doesn't need this.
  # In fact, there seems to be a problem if I used the Threads package
  # and try using this line, so I'm just skipping it entirely for OS X.
  if (NOT APPLE)
    set(SDL2_LIBRARY_TEMP ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
    get_property(INTERFACE_LINK_LIBRARIES TARGET SDL2 PROPERTY INTERFACE_LINK_LIBRARIES)
    set_property(TARGET SDL2 PROPERTY
      INTERFACE_LINK_LIBRARIES ${INTERFACE_LINK_LIBRARIES} ${CMAKE_THREADS_LIBS_INIT})
  endif ()

  # For MinGW library
  if (MINGW)
    set(SDL2_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL2_LIBRARY_TEMP})
    get_property(INTERFACE_LINK_LIBRARIES TARGET SDL2 PROPERTY INTERFACE_LINK_LIBRARIES)
    set_property(TARGET SDL2 PROPERTY
      INTERFACE_LINK_LIBRARIES ${INTERFACE_LINK_LIBRARIES} ${MINGW32_LIBRARY})
  endif ()

  # Set the final string here so the GUI reflects the final state.
  set(SDL2_LIBRARY ${SDL2_LIBRARY_TEMP} CACHE STRING "Where the SDL2 Library can be found")
  # Set the temp variable to INTERNAL so it is not seen in the CMake GUI
  set(SDL2_LIBRARY_TEMP "${SDL2_LIBRARY_TEMP}" CACHE INTERNAL "")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)
