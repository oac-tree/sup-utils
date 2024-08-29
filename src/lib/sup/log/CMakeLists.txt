add_library(sup-log SHARED "")
add_library(sup-utils::sup-log ALIAS sup-log) # alias for build-tree usage

target_compile_features(sup-log PRIVATE cxx_std_11)

set_target_properties(sup-log PROPERTIES
  EXPORT_NAME sup-log
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
)

target_include_directories(sup-log PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_sources(sup-log
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/basic_logger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/default_loggers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/log_severity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
)

# -- Installation --

install(TARGETS sup-log EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
  base_types.h
  basic_logger.h
  default_loggers.h
  log_severity.h
  logger_t.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/log
)
