# Settings of CMake project

include(GNUInstallDirs)
include(CTest)

get_filename_component(SUP_UTILS_PROJECT_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

set(LIBVERSION ${CMAKE_PROJECT_VERSION})
set(LIBSOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})

# detecting CODAC environment
if (SUP_UTILS_CODAC)
  if (DEFINED ENV{CODAC_ROOT})
    message(STATUS "CODAC environment detected at $ENV{CODAC_ROOT}")
  else()
    message(FATAL "No CODAC environment detected")
  endif()
else()
  message(STATUS "Compiling without CODAC")
endif()

# build settings
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Directories
if (SUP_UTILS_CODAC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SUP_UTILS_PROJECT_DIR}/target/bin)
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
