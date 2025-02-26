if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for sup-utils
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(sup-utils-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
    sup-cli=${CMAKE_CURRENT_SOURCE_DIR}/src/lib/sup/cli
    sup-codec=${CMAKE_CURRENT_SOURCE_DIR}/src/lib/sup/codec
    sup-log=${CMAKE_CURRENT_SOURCE_DIR}/src/lib/sup/log
    sup-xml=${CMAKE_CURRENT_SOURCE_DIR}/src/lib/sup/xml
  TARGETS sup-cli sup-codec sup-log sup-xml
)

get_target_property(SUP_UTILS_INCLUDE_DIRECTORIES sup-xml INCLUDE_DIRECTORIES)
target_include_directories(sup-utils-cpptest PUBLIC ${SUP_UTILS_INCLUDE_DIRECTORIES})

target_link_libraries(sup-utils-cpptest PUBLIC LibXml2::LibXml2)
