add_library(sup-platform SHARED "")
add_library(sup-utils::sup-platform ALIAS sup-platform) # alias for build-tree usage

set_target_properties(sup-platform PROPERTIES
  EXPORT_NAME sup-platform
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
)

target_include_directories(sup-platform PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_sources(sup-platform
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host.cpp
    ${CMAKE_CURRENT_LIST_DIR}/load_library.cpp
)

# -- Installation --

install(TARGETS sup-platform EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  host.h
  load_library.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/platform
)
