cmake_minimum_required(VERSION 3.5)

project(sup-utils-project-all VERSION 1.0.0)

option(NO_CODAC "Don't look for the presence of CODAC environment" OFF)

add_subdirectory(src)
add_subdirectory(test)
