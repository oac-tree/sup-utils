add_executable(unit-tests)

target_sources(unit-tests PRIVATE
  base64_tests.cpp
  basic_logger_tests.cpp
  command_line_option_tests.cpp
  command_line_parser_tests.cpp
  command_line_utils_tests.cpp
  decorate_with_tests.cpp
  default_loggers_tests.cpp
  log_severity_tests.cpp
  logger_t_tests.cpp
  tree_data_tests.cpp
  tree_data_parse_tests.cpp
  tree_data_serialize_tests.cpp
  tree_data_validate_tests.cpp
  unit_test_helper.cpp
  xml_exception_tests.cpp
)

target_link_libraries(unit-tests
  PRIVATE
  GTest::GTest
  GTest::Main
  GTest::gmock
  sup-cli
  sup-codec
  sup-log
  sup-xml
  LibXml2::LibXml2
)

set_target_properties(unit-tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

gtest_discover_tests(unit-tests)
