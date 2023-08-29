# Settings of CMake project

include(GNUInstallDirs)
include(CTest)

# Detecting CODAC environment
if(NOT COA_NO_CODAC)
  find_package(CODAC OPTIONAL_COMPONENTS site-packages Python MODULE)
endif()
if (CODAC_FOUND)
  # Append CODAC_CMAKE_PREFIXES to cmake seard directories, this helps cmake find packages installed in the CODAC enviorenment
  list(APPEND CMAKE_PREFIX_PATH ${CODAC_CMAKE_PREFIXES})

  # If CODAC module provides python executable, override Python3_EXECUTABLE with it
  if(CODAC_Python_FOUND AND NOT Python3_EXECUTABLE)
    set(Python3_EXECUTABLE ${CODAC_PYTHON_EXECUTABLE})
  endif()

  if (CODAC_CICD)
    # When operating inside a CODAC CICD system build the documentation
    set(COA_BUILD_DOCUMENTATION ON)
  endif()
else()
  message(STATUS "Compiling without CODAC")
endif()

# Build settings
if(COVERAGE)
  # On coverage builds  alsways skip building docs and build tests
  set(COA_BUILD_DOCUMENTATION OFF)
  set(COA_BUILD_TESTS ON)
endif()

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

set(LIBVERSION ${CMAKE_PROJECT_VERSION})
set(LIBSOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR})

# Directories
if (NOT DEFINED TEST_OUTPUT_DIRECTORY)
  set(TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test_bin)
endif()

file(MAKE_DIRECTORY ${TEST_OUTPUT_DIRECTORY})
