project(KiWi)
cmake_minimum_required(VERSION 2.8)

# Point to our own cmake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake-modules)

# Define this as top-level
set(KIWI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Add global definitions
add_definitions("-Wno-unused-functions -Wall")

# Add Debug definitions
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

option(DEBUG_GEOMETRY_CHANGE "Print a lot of messages when calculating composed geometries" OFF)
if (DEBUG_GEOMETRY_CHANGE)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_GEOMETRY_CHANGE")
endif()

option(DEBUG_PRINT_GEOMETRY_RECTANGLE "Display composed geometry rectangles of widgets" OFF)
if (DEBUG_PRINT_GEOMETRY_RECTANGLE)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_PRINT_GEOMETRY_RECTANGLE")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -fvisibility=hidden -std=c89 -pedantic-errors")
endif(CMAKE_COMPILER_IS_GNUCC)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples)

# Install KiWiConfig stuff
set(INCLUDE_INSTALL_DIR include/KiWi)
set(LIB_INSTALL_DIR lib)
export(PACKAGE KiWi)
if (UNIX)
  set(CMAKECONFIG_INSTALL_DIR "lib/cmake/${PROJECT_NAME}" CACHE STRING "Where ${PROJECT_NAME}-config.cmake and companions will be installed")
elseif(WIN32)
  set(CMAKECONFIG_INSTALL_DIR "cmake"  CACHE STRING "Where ${PROJECT_NAME}-config.cmake and companions will be installed")
elseif(APPLE)
  set(CMAKECONFIG_INSTALL_DIR "cmake" CACHE STRING "Where ${PROJECT_NAME}-config.cmake and companions will be installed")
endif()

include(CMakePackageConfigHelpers)
configure_package_config_file(KiWiConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/tmpkiwiconfig
                              INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
                              PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/KiWiConfigVersion.cmake
                                 VERSION 0.0.1
                                 COMPATIBILITY SameMajorVersion)
                                 
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/KiWiConfigVersion.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
        
install(FILES       
        ${CMAKE_CURRENT_BINARY_DIR}/tmpkiwiconfig
        RENAME KiWiConfig.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR} )

# install KiWiTargets.cmake
install(EXPORT KiWiTargets DESTINATION ${CMAKECONFIG_INSTALL_DIR})
