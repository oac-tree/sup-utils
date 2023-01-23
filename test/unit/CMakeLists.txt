add_executable(unit-tests)

target_compile_features(unit-tests PRIVATE cxx_std_11)

target_sources(unit-tests PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/basic_logger_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/command_line_option_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/command_line_parser_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/default_loggers_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/log_severity_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/logger_t_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tree_data_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tree_data_parse_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tree_data_serialize_tests.cpp
  ${CMAKE_CURRENT_LIST_DIR}/unit_test_helper.cpp
  ${CMAKE_CURRENT_LIST_DIR}/xml_exception_tests.cpp
)

find_package(GTest REQUIRED)
find_package(LibXml2 REQUIRED)

include(GoogleTest)

target_link_libraries(unit-tests
    PRIVATE
        GTest::GTest
        GTest::Main
        sup-log
        sup-xml
        sup-cli
        LibXml2::LibXml2
)

set_target_properties(unit-tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(unit-tests)
