#
# project: n_closure
#

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

cmake_minimum_required(VERSION 3.0)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

if (NOT FIPS_IMPORT)
    fips_setup(PROJECT n_closure)
        fips_add_subdirectory(examples)
        fips_add_subdirectory(tests)
    fips_finish()
endif()
