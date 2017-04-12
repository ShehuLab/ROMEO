PROJECT(Runner CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

IF(NOT CMAKE_BUILD_TYPE)
 set(CMAKE_BUILD_TYPE Release)
ENDIF()

#############################################################################
#Compiler flags for normal (debug) and optimized (release) builds
#
#

SET(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -std=c++11" CACHE STRING "Flags" FORCE)

IF(NOT MSVC_IDE)

  SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -std=c++11" CACHE STRING  
      "Flags used by the C++ compiler during debug builds." FORCE)
  SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -std=c++11" 
      CACHE STRING "Flags used by the C++ compiler during release builds." FORCE) 
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -std=c++11" CACHE STRING
      "Flags used by the C++ compiler during release builds with debug information." FORCE) 

  SET(CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE STRING
      "Flags used by the C compiler during debug builds." FORCE)
  SET(CMAKE_C_FLAGS_RELEASE "-O3 -Wno-unused-result -DNDEBUG" CACHE STRING
      "Flags used by the C compiler during release builds." FORCE)
  SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -g" CACHE STRING
      "Flags used by the C compiler during release builds with debug information." FORCE)
ENDIF(NOT MSVC_IDE)

IF(APPLE)
	LIST(APPEND CMAKE_SHARED_LINKER_FLAGS "-single_module")
ENDIF(APPLE)

ADD_DEFINITIONS(-D_USE_MATH_DEFINES)


#############################################################################
INCLUDE_DIRECTORIES(src)
AUX_SOURCE_DIRECTORY(src/Utils      SRC_UTILS)
AUX_SOURCE_DIRECTORY(src/Components SRC_COMPONENTS)
AUX_SOURCE_DIRECTORY(src/Components/CfgAccepters SRC_CFG_ACCEPTERS)
AUX_SOURCE_DIRECTORY(src/Components/CfgManagers SRC_CFG_MANAGERS)
AUX_SOURCE_DIRECTORY(src/Components/CfgDistances SRC_CFG_DISTANCES)
AUX_SOURCE_DIRECTORY(src/Components/CfgEnergyEvaluators SRC_CFG_ENERGY_EVALUATORS)
AUX_SOURCE_DIRECTORY(src/Components/CfgForwardKinematics SRC_CFG_FORWARD_KINEMATICS)
AUX_SOURCE_DIRECTORY(src/Components/CfgImprovers SRC_CFG_IMPROVERS)
AUX_SOURCE_DIRECTORY(src/Components/CfgOffspringGenerators SRC_CFG_OFFSPRING_GENERATORS)
AUX_SOURCE_DIRECTORY(src/Components/CfgProjectors SRC_CFG_PROJECTORS)
AUX_SOURCE_DIRECTORY(src/Components/CfgSamplers SRC_CFG_SAMPLERS)
AUX_SOURCE_DIRECTORY(src/Components/EdgeCostEvaluators SRC_EDGE_COST_EVALUATORS)

AUX_SOURCE_DIRECTORY(src/Planners   SRC_PLANNERS)
AUX_SOURCE_DIRECTORY(src/Programs   SRC_PROGRAMS)
AUX_SOURCE_DIRECTORY(src/Setup      SRC_SETUP)
AUX_SOURCE_DIRECTORY(src/Plugin2D     SRC_PLUGIN2D)
AUX_SOURCE_DIRECTORY(src/PluginRosetta     SRC_PLUGIN_ROSETTA)
   
 
ADD_EXECUTABLE(Runner
		${SRC_UTILS}
		${SRC_COMPONENTS}
		${SRC_CFG_ACCEPTERS}
		${SRC_CFG_MANAGERS}
		${SRC_CFG_DISTANCES}
		${SRC_CFG_ENERGY_EVALUATORS}
		${SRC_CFG_FORWARD_KINEMATICS}
		${SRC_CFG_IMPROVERS}
		${SRC_CFG_OFFSPRING_GENERATORS}
		${SRC_CFG_PROJECTORS}
		${SRC_CFG_SAMPLERS}
		${SRC_EDGE_COST_EVALUATORS}
		${SRC_PLANNERS}
		${SRC_SETUP}
		${SRC_PROGRAMS}
		${SRC_PLUGIN2D}
		${SRC_PLUGIN_ROSETTA}
	      )



#############################################################################
#Find OpenGL and GLUT
#
INCLUDE(FindOpenGL)
IF(NOT OPENGL_INCLUDE_DIR)
  SET(OPENGL_INCLUDE_DIR GL CACHE STRING "OpenGL directory for header file" FORCE)
ENDIF(NOT OPENGL_INCLUDE_DIR)

INCLUDE(FindGLUT)
IF(NOT GLUT_INCLUDE_DIR)
  SET(GLUT_INCLUDE_DIR GL CACHE STRING "Glut header file" FORCE)
ENDIF(NOT GLUT_INCLUDE_DIR)
IF (NOT GLUT_glut_LIBRARY)
  IF (WIN32 AND NOT CYGWIN)
    SET(GLUT_glut_LIBRARY glut32 CACHE STRING "Glut library for win32" FORCE)
  ENDIF(WIN32 AND NOT CYGWIN)
ENDIF (NOT GLUT_glut_LIBRARY)

INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR} /usr/local/include/bullet)


IF(GLUT_glut_LIBRARY)
  SET(INTERACTIVE_LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${GLUT_glut_LIBRARY})
ELSE (GLUT_glut_LIBRARY)
  SET(INTERACTIVE_LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
ENDIF(GLUT_glut_LIBRARY)


TARGET_LINK_LIBRARIES(Runner   ${INTERACTIVE_LIBS} m pthread)

FIND_LIBRARY(DL_LIB dl /usr/lib/ /usr/local/lib/ ./lib/ ${LIBRARY_OUTPUT_PATH}) 
IF(DL_LIB)
   TARGET_LINK_LIBRARIES(Runner ${DL_LIB})
ENDIF(DL_LIB)  


