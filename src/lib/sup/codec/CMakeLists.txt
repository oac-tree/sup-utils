set(library_name sup-codec)
set(export_name sup-codec)

add_library(${library_name} SHARED "")
add_library(sup-utils::${export_name} ALIAS ${library_name}) # alias for build-tree usage

target_compile_features(${library_name} PRIVATE cxx_std_11)

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
  base64.cpp
)

add_subdirectory(modp_b64)

# -- Installation --

install(TARGETS ${library_name} EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  base64.h
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/codec
)
