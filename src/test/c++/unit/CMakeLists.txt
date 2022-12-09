add_executable(unit-tests)

target_compile_features(unit-tests PRIVATE cxx_std_11)

target_sources(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/basic_logger_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/default_loggers_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/log_severity_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/logger_t_tests.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tree_data_parse_tests.cpp
)

find_package(GTest REQUIRED)

target_link_libraries(unit-tests
    PRIVATE
        GTest::GTest
        GTest::Main
        gmock
        sup-log
        sup-xml
)

gtest_discover_tests(unit-tests)
