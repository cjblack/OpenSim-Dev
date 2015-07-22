#-----------------------------------------------------------------------------
# www.simtk.org
# Generic CMake file for building an OpenSim project
# Author: The OpenSim Team
# Edited by Chris Black
#-----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.6)
# Create Project (Automatically creates variables Project_BINARY_DIR and Project_SOURCE_DIR}
PROJECT (ProjectName)


# Settings.
# ---------
SET(TARGET ExecuteFileName CACHE TYPE STRING)
#SET(OPENSIM_INSTALL_DIR $ENV{OPENSIM_HOME} CACHE PATH "Top-level directory of OpenSim install")

#-----------------------------------------------------------------------------
#Setup the variable OPENSIM_INSTALL_DIR to the Environment Variable
SET(OPENSIM_INSTALL_DIR "/Users/username/OpenSim-InstallDir/“ CACHE PATH "Top-level directory of OpenSim install")
# To add Debug feature add ";Debug" after Release on the line below
SET(CMAKE_CONFIGURATION_TYPES "RelWithDebInfo;Release"
      CACHE STRING "Semicolon separated list of supported configuration types, only supports Release and RelWithDebInfo, anything else will be ignored." FORCE )
#-----------------------------------------------------------------------------
# Setup Targets
SET(TARGET ExecuteFileName CACHE STRING "The Name of the executable")
FILE(GLOB SOURCE_FILES *.h *.cpp)
#-----------------------------------------------------------------------------
# Find and hook up to OpenSim.
# ----------------------------
# OpenSim uses C++11 language features.
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # Using C++11 on OSX requires using libc++ instead of libstd++.
    # libc++ is an implementation of the C++ standard library for OSX.
    if(APPLE)
        if(XCODE)
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
            if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
            endif()
        endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
endif()
FIND_PACKAGE(OpenSim REQUIRED PATHS "${OPENSIM_INSTALL_DIR}")
#-----------------------------------------------------------------------------
# Setup Headers to Include
SET(SIMTK_HEADERS_DIR ${OPENSIM_INSTALL_DIR}/sdk/include/SimTK/simbody)
SET(OPENSIM_HEADERS_DIR ${OPENSIM_INSTALL_DIR}/sdk/include)
SET(OPENSIM_TOOLS_HEADERS_DIR ${OPENSIM_INSTALL_DIR}/sdk/include/OpenSim/Tools)
INCLUDE_DIRECTORIES(${OpenSim_INCLUDE_DIRS} ${SIMTK_HEADERS_DIR} ${OPENSIM_HEADERS_DIR} ${OPENSIM_TOOLS_HEADERS_DIR})
#-----------------------------------------------------------------------------
# Setup Links
SET(OPENSIM_LIBS_DIR ${OPENSIM_INSTALL_DIR}/lib)
SET(OPENSIM_DLLS_DIR ${OPENSIM_INSTALL_DIR}/bin)
LINK_DIRECTORIES( ${OpenSim_LIB_DIR} ${OPENSIM_LIBS_DIR} ${OPENSIM_DLLS_DIR})
#-----------------------------------------------------------------------------
# Namespace
ADD_EXECUTABLE(${TARGET} ${SOURCE_FILES})
#-----------------------------------------------------------------------------



TARGET_LINK_LIBRARIES(${TARGET} ${OpenSim_LIBRARIES})
#-----------------------------------------------------------------------------
# This block copies the additional files into the running directory
# For example vtp, obj files. Add to the end for more extentions
FILE(GLOB DATA_FILES *.vtp *.obj *.osim)
FOREACH (dataFile ${DATA_FILES})
    ADD_CUSTOM_COMMAND(
        TARGET ${TARGET}
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy
        ${dataFile}
        ${CMAKE_BINARY_DIR})
ENDFOREACH (dataFile)
