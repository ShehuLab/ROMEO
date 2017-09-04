###############################################################################
# Find Rosetta
# CMake to include Rosetta include files and liibraries
#

cmake_policy(SET CMP0009 NEW)
cmake_policy(SET CMP0011 NEW)

set (ROSETTA_DIR $ENV{RUNNER_ROSETTA_DIR})
message(STATUS "Rosetta root directory is ${ROSETTA_DIR}" )

## verify that an include file we need exists

find_path(ROSETTA_INCLUDE_DIR NAMES core/init/init.hh
          PATHS ${ROSETTA_DIR}/main/source/src)

if (NOT ROSETTA_INCLUDE_DIR)
  message(FATAL_ERROR "Can not find include files with supplies path")
endif()

set (ROSETTA_PLATFORM linux)

find_path(ROSETTA_INCLUDE_DIR_PLAT NAMES_PLATFORM platform/types.hh
          PATHS ${ROSETTA_DIR}/main/source/src/platform/${ROSETTA_PLATFORM} )

find_path(ROSETTA_BOOST_DIR NAMES boost/shared_ptr.hpp
     PATHS ${ROSETTA_DIR}/main/source/external/boost_1_55_0)
message (STATUS "Rosetta boost dir is: ${ROSETTA_BOOST_DIR}.")

file(GLOB_RECURSE libloc ${ROSETTA_DIR}/libprotocols.1.so)

get_filename_component(ROSETTA_LIBRARY_DIR ${libloc} DIRECTORY)

set (ROSETTA_INCLUDE_DIRS ${ROSETTA_INCLUDE_DIR}
                          ${ROSETTA_INCLUDE_DIR_PLAT} 
                          ${ROSETTA_BOOST_DIR} )

set (ROSETTA_LIBNAMES basic core.1 core.2 core.3 core.4 core.5
                      numeric utility devel protocols.1 
                      protocols_a.2 protocols.3 protocols.7
                      ObjexxFCL sqlite3)
