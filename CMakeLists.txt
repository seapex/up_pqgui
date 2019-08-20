cmake_minimum_required (VERSION 2.8)
project (up_pqgui)

# The version number.
set (_VERSION_MAJ 1)
set (_VERSION_MIN 0)
#set (_VERSION_PAT 0)

# configure a header file to pass some of the CMake settings to the source code
configure_file ( "${PROJECT_SOURCE_DIR}/config_gui.h.in"
                 "${PROJECT_BINARY_DIR}/config_gui.h" )

# add the binary tree to the search path for include files so that we will find config.h
include_directories ("${PROJECT_BINARY_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}")

# add the src/ to the search path for src files
add_subdirectory (src bin)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${_VERSION_PATCH}")
include (CPack)
