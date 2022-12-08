set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -pedantic")
if (COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage --coverage")
endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_library(sup-log SHARED "")
add_library(sup-utils::sup-log ALIAS sup-log) # alias for build-tree usage

target_compile_features(sup-log PRIVATE cxx_std_11)

target_include_directories(sup-log PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

add_subdirectory(sup)

# -- Installation --

set_target_properties(sup-log PROPERTIES
    EXPORT_NAME sup-log
    SOVERSION ${LIBSOVERSION}
    VERSION ${LIBVERSION}
)

install(TARGETS sup-log EXPORT sup-utils-targets LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
        sup/log/basic_logger.h
        sup/log/default_loggers.h
        sup/log/log_severity.h
        sup/log/logger_t.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sup/log
)
