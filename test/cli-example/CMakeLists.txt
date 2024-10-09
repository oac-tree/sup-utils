add_executable(cli-example)

target_sources(cli-example PRIVATE main.cpp)
target_link_libraries(cli-example PRIVATE sup-cli)

set_target_properties(cli-example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

