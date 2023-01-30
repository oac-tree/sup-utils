# -----------------------------------------------------------------------------
# Installation
# Credits to https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right,
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/Exporting-and-Importing-Targets
# -----------------------------------------------------------------------------

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/sup-utils)

# exporting targets to a script and installing it
install(EXPORT sup-utils-targets FILE sup-utils-targets.cmake NAMESPACE sup-utils:: DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Exporting
# -----------------------------------------------------------------------------

# Add all targets to the build-tree export set
export(TARGETS sup-log sup-xml sup-cli NAMESPACE sup-utils:: FILE "${PROJECT_BINARY_DIR}/sup-utils-targets.cmake")

# Export the package for use from the build-tree (goes to $HOME/.cmake)
if(SUP_UTILS_EXPORT_BUILDTREE)
  set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
  export(PACKAGE sup-utils)
endif()

# -----------------------------------------------------------------------------
# Creating and installing sup-utils-config.cmake
# -----------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

# to use in the build tree
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/sup-utils-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/sup-utils-config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# to use in install tree
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sup-utils-config.cmake DESTINATION ${INSTALL_CONFIGDIR})

# -----------------------------------------------------------------------------
# Create and install sup-utils-config-version.cmake file
# -----------------------------------------------------------------------------

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/sup-utils-config-version.cmake VERSION
    ${PROJECT_VERSION} COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sup-utils-config-version.cmake DESTINATION ${INSTALL_CONFIGDIR})
