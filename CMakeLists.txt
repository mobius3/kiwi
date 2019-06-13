project(KiWi)
cmake_minimum_required(VERSION 2.8)

# Point to our own cmake modules
set(KIWI_ROOT ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "")
list(APPEND CMAKE_MODULE_PATH ${KIWI_ROOT}/cmake-modules)

# Define this as top-level
set(KIWI_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Add global definitions
add_definitions("-Wall")

# add option to build examples
option(KIWI_BUILD_EXAMPLES "Build examples in examples/ directory" ON)

# Add Debug definitions
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DNDEBUG")

if(WIN32)
	set(SDL2_ROOT "/sdl2" CACHE STRING "Root of the SDL2 installation")
endif(WIN32)

# Add subdirectories
add_subdirectory(src)
if(KIWI_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()

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
