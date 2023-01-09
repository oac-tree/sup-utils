find_package(LibXml2 REQUIRED)

add_library(sup-xml SHARED "")
add_library(sup-utils::sup-xml ALIAS sup-xml) # alias for build-tree usage

target_compile_features(sup-xml PRIVATE cxx_std_11)

set_target_properties(sup-xml PROPERTIES
  EXPORT_NAME sup-xml
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
)

target_include_directories(sup-xml PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_sources(sup-xml
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/exceptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tree_data_parser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tree_data_parser_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tree_data_serialize_utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tree_data.cpp
    ${CMAKE_CURRENT_LIST_DIR}/xml_utils.cpp
)

target_link_libraries(sup-xml PRIVATE LibXml2::LibXml2)

# -- Installation --

install(TARGETS sup-xml EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  exceptions.h
  tree_data_parser.h
  tree_data_serialize_utils.h
  tree_data.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/xml
)
