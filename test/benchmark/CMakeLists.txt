include(FindPackageMessage)

find_package(benchmark QUIET)
# Print the benchmark found message
set(BENCHMARK_MESSAGE "Could NOT find Google Benchmark, skipping benchmark tests")
if(benchmark_FOUND)
  set(BENCHMARK_MESSAGE "Found Google Benchmark: ${benchmark_DIR} (found version \"${benchmark_VERSION}\")")
endif()
find_package_message(benchmark "${BENCHMARK_MESSAGE}" "[${BENCHMARK_MESSAGE}]")

if(NOT benchmark_FOUND)
  message(DEBUG "Google Benchmark not found, skipping benchmark tests")
  return()
endif()

set(benchmark-tests sup-utils-benchmark-tests)

add_executable(${benchmark-tests})

set_target_properties(${benchmark-tests} PROPERTIES OUTPUT_NAME "benchmark-tests")
set_target_properties(${benchmark-tests} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

target_sources(${benchmark-tests}
  PRIVATE
    main.cpp
    tree_data_benchmark.cpp
    tree_data_parse_benchmark.cpp
    tree_data_serialize_benchmark.cpp
    tree_shape_fixtures.cpp
)

target_link_libraries(${benchmark-tests}
  PRIVATE
    benchmark::benchmark
    sup-xml
)
