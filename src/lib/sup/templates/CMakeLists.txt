add_library(sup-templates INTERFACE)
add_library(sup-utils::sup-templates ALIAS sup-templates) # alias for build-tree usage

set_target_properties(sup-templates PROPERTIES
  EXPORT_NAME sup-templates
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
)

target_include_directories(sup-templates INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..> )

# -- installation --

install(TARGETS sup-templates EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  decorate_with.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/templates
)
