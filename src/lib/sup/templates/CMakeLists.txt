add_library(sup-templates INTERFACE)
add_library(sup-utils::sup-templates ALIAS sup-templates) # alias for build-tree usage

# target_compile_features(sup-templates PRIVATE cxx_std_11)

set_target_properties(sup-templates PROPERTIES
  EXPORT_NAME sup-templates
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
)

# target_include_directories(sup-templates INTERFACE
# .
# )

# -- installation --

install(TARGETS sup-templates EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  decorate_with.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/templates
)
