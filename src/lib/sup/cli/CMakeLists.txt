set(library_name sup-cli)
set(export_name sup-cli)

add_library(${library_name} SHARED "")
add_library(sup-utils::${export_name} ALIAS ${library_name}) # alias for build-tree usage

target_compile_features(${library_name} PRIVATE cxx_std_14)

set_target_properties(${library_name} PROPERTIES
  EXPORT_NAME ${export_name}
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
)

target_include_directories(${library_name} PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_sources(${library_name} PRIVATE
  command_line_option.cpp
  command_line_option.h
  command_line_parser.cpp
  command_line_parser.h
  command_line_utils.cpp
  command_line_utils.h
)

# -- Installation --

install(TARGETS ${library_name} EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  command_line_option.h
  command_line_parser.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/cli
)
