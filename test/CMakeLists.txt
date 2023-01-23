if (COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fno-inline -fprofile-arcs -ftest-coverage --coverage")
endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_subdirectory(unit)

file(WRITE ${TEST_OUTPUT_DIRECTORY}/test.sh
"#!/bin/bash
" ${TEST_OUTPUT_DIRECTORY} "/unit-tests \"$@\"
exit $?
")

configure_file(${TEST_OUTPUT_DIRECTORY}/test.sh ${TEST_OUTPUT_DIRECTORY}/test.sh 
               FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ)
