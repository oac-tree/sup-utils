if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

message(STATUS "Configuring Parasoft integration")

if(NOT PARASOFT_DIR)
  if(NOT CODAC_FOUND)
    message(FATAL_ERROR "Parasoft install location is unknown, set PARASOFT_DIR to specify one")
  endif()

  # Default to CODAC parasoft installation location in /opt
  set(PARASOFT_DIR /opt/parasoft CACHE PATH "Parasoft install location")
endif()

# Append parasoft cmake integration location to module path
list(APPEND CMAKE_MODULE_PATH "${PARASOFT_DIR}/cpptest/integration/cmake")

# Enable parasoft project generation
set(CPPTEST_PROJECT ON)

# Include Parasoft integration module
include(cpptest-project)

# Add parasoft project for sup-utils
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(sup-utils-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
    sup-cli=${CMAKE_SOURCE_DIR}/src/lib/sup/cli
    sup-codec=${CMAKE_SOURCE_DIR}/src/lib/sup/codec
    sup-log=${CMAKE_SOURCE_DIR}/src/lib/sup/log
    sup-xml=${CMAKE_SOURCE_DIR}/src/lib/sup/xml
  TARGETS sup-cli sup-codec sup-log sup-xml
)

get_target_property(SUP_UTILS_INCLUDE_DIRECTORIES sup-xml INCLUDE_DIRECTORIES)
target_include_directories(sup-utils-cpptest PUBLIC ${SUP_UTILS_INCLUDE_DIRECTORIES})

target_link_libraries(sup-utils-cpptest PUBLIC LibXml2::LibXml2)
