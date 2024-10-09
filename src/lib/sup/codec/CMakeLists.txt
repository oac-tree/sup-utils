add_library(sup-codec SHARED "")
add_library(sup-utils::sup-codec ALIAS sup-codec) # alias for build-tree usage

set_target_properties(sup-codec PROPERTIES
  EXPORT_NAME sup-codec
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
)

target_include_directories(sup-codec PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_sources(sup-codec PRIVATE
  base64.cpp
)

add_subdirectory(modp_b64)

# -- Installation --

install(TARGETS sup-codec EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  base_types.h
  base64.h
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/codec
)
