PROJECT(mpv)

# CMake 2.4.7 includes useful new macros, including FindPkgConfig.cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

IF(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
ENDIF()

# If CMake > 2.6.0
IF(CMAKE_MAJOR_VERSION GREATER 1 AND
   CMAKE_MINOR_VERSION GREATER 4)
    IF(WIN32)
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
            CACHE STRING "Where libraries go" FORCE)
    ELSE()
        SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
            CACHE STRING "Where libraries go" FORCE)
    ENDIF()
    SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
        CACHE STRING "Where archives go" FORCE)
    SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
        CACHE STRING "Where binaries and .dll files go" FORCE)
ELSE()
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin 
        CACHE STRING "Where binaries go" FORCE)
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib
        CACHE STRING "Where libraries go" FORCE)
ENDIF()

INCLUDE(mpv_macros)

#==========================================================
# pkg-config 
# (helpful utility for finding libraries on linux and friends)
#==========================================================

FIND_PACKAGE(PkgConfig)

#==========================================================
# Required Dependencies
#==========================================================

#----------------------------
# Threading library
#----------------------------

# if extra compiler arguments are required to enable threading, the 
# following cmake command will figure it all out and place the results 
# in CMAKE_THREAD_LIBS_INIT
FIND_PACKAGE(Threads)


#----------------------------
# PDL - plugin loader library
#----------------------------

IF(EXISTS "${CMAKE_SOURCE_DIR}/pdl" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/pdl")
    # use the provided pdl
    ADD_SUBDIRECTORY(pdl)
    
    SET(PDL_FOUND "YES")
    SET(PDL_LIBRARIES    pdl)
    SET(PDL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/pdl/include)
ELSE()
    # search for an installed pdl
    FIND_PATH(PDL_INCLUDE_DIR DynamicLoader.hpp
        DOC "The PDL include path"
        NO_DEFAULT_PATH
    )
    FIND_LIBRARY(PDL_LIBRARY NAMES pdl
        DOC "The PDL library file"
        NO_DEFAULT_PATH
    )
    SET(PDL_FOUND "NO")
    IF(PDL_LIBRARY AND PDL_INCLUDE_DIR)
        SET(PDL_FOUND "YES")
        SET(PDL_LIBRARIES    ${PDL_LIBRARY})
        SET(PDL_INCLUDE_DIRS ${PDL_INCLUDE_DIR})
    ELSE(PDL_LIBRARY AND PDL_INCLUDE_DIR)
        MESSAGE(FATAL_ERROR "Could not find PDL")
    ENDIF(PDL_LIBRARY AND PDL_INCLUDE_DIR)
ENDIF()

#----------------------------
# CIGI Class Library
#----------------------------

# When compiling with Visual Studio search for a precompiled version of
# the CCL in the 3rdPartyMPV folder.
IF(WIN32 AND MSVC)
    SET(CCL_ROOT ${PROJECT_SOURCE_DIR}/../3rdPartyMPV/ccl
        CACHE PATH "The location of the CCL install prefix")
ELSE()
    SET(CCL_ROOT ""
        CACHE PATH "The location of the CCL install prefix")
ENDIF()

FIND_PACKAGE(CCL)
IF(NOT CCL_FOUND)
    MESSAGE("The CIGI Class library was not found, but is required")
ELSE()
    ADD_DEFINITIONS(${CCL_DEFINITIONS})
ENDIF()

#----------------------------
# Boost
#----------------------------


IF(MSVC)
    # Hooray for declspec!
	SET(Boost_USE_STATIC_LIBS OFF)
    ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
ELSE()
	SET(Boost_USE_STATIC_LIBS ON)
ENDIF()
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost COMPONENTS signals)

IF(NOT Boost_FOUND)
    MESSAGE("Boost is required")
ELSE()
    ADD_DEFINITIONS(-DHAVE_BOOST)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF()

#----------------------------
# SDL
#----------------------------

# When compiling with Visual Studio search for a precompiled version
# of the SDL in the 3rdPartyMPV folder.
#IF(WIN32 AND MSVC)
#    SET(SDL_ROOT ${PROJECT_SOURCE_DIR}/../3rdPartyMPV/SDL-1.2.13
#        CACHE PATH "A location to look for the SDL")
#ELSE()
#    SET(SDL_ROOT ""
#        CACHE PATH "A location to look for the SDL")
#ENDIF()

#IF(SDL_ROOT)
#    IF(CMAKE_MAJOR_VERSION GREATER 1 AND
#            CMAKE_MINOR_VERSION GREATER 4)
#        SET(CMAKE_PREFIX_PATH ${SDL_ROOT})
#    ELSE()
#        # These stopped working in CMake 2.6.0, probably a bug
#        SET(CMAKE_INCLUDE_PATH ${SDL_ROOT}/include)
#        SET(CMAKE_LIBRARY_PATH ${SDL_ROOT}/lib)
#    ENDIF()
#ENDIF()

#FIND_PACKAGE(SDL)

#IF(NOT SDL_FOUND)
#    MESSAGE("The SDL was not found, but is required")
#ENDIF(NOT SDL_FOUND)

#----------------------------
# OSG
#----------------------------

# Allow the user to specify their OSG location at CMake
# configuration time instead of having to make them rely
# on the OSG_DIR environment variable.
SET(OSG_ROOT "" CACHE PATH "A location to look for OSG")
IF(OSG_ROOT)
    IF(CMAKE_MAJOR_VERSION GREATER 1 AND
            CMAKE_MINOR_VERSION GREATER 4)
        SET(CMAKE_PREFIX_PATH ${OSG_ROOT})
    ELSE()
        # These stopped working in CMake 2.6.0, probably a bug
        SET(CMAKE_INCLUDE_PATH ${OSG_ROOT}/include)
        SET(CMAKE_LIBRARY_PATH ${OSG_ROOT}/lib)
    ENDIF()
ENDIF()

FIND_PACKAGE(osg)
FIND_PACKAGE(osgDB)
FIND_PACKAGE(osgGA)
FIND_PACKAGE(osgUtil)
FIND_PACKAGE(osgText)
FIND_PACKAGE(osgSim)
FIND_PACKAGE(osgViewer)
FIND_PACKAGE(OpenThreads)

#----------------------------
# OpenGL
#----------------------------

FIND_PACKAGE(OpenGL)

#==========================================================
# Optional Dependencies
#==========================================================

#----------------------------
# Jpeg & FFmpeg (for pluginVideoCapture)
#----------------------------

FIND_PACKAGE(JPEG)
IF(JPEG_FOUND)
    ADD_DEFINITIONS(-DHAVE_JPEG)
ENDIF()

# we use a boolean here to maintain the --with-ffmpeg style of autoconf
OPTION(USE_FFMPEG "Use the ffmpeg library to add capture capability to pluginVideoCapture" FALSE)
IF(USE_FFMPEG)
    FIND_PACKAGE(FFmpeg)
    IF(FFMPEG_FOUND)
        ADD_DEFINITIONS(-DHAVE_FFMPEG)
    ENDIF()
ENDIF()

#----------------------------
# osgAL
#----------------------------

pkg_check_modules(OSGAL "osgal >= 0.6")

#==========================================================
# Build options
#==========================================================

OPTION(USE_PAPI
    "Use PAPI library for timers (requires kernel patches)" FALSE)
IF(USE_PAPI)
    ADD_DEFINITIONS(-DHAVE_PAPI)
ENDIF(USE_PAPI)

#==========================================================
# Global Preprocessor Definitions
#==========================================================

IF(CMAKE_COMPILER_IS_GNUCXX)
    ADD_DEFINITIONS(-Wall -fmessage-length=0)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(MSVC)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ENDIF(MSVC)

IF(WIN32)
    # Use shared library builds
    ADD_DEFINITIONS(-DMPVCMNOSG_DLL)
    ADD_DEFINITIONS(-DMPVCMN_DLL)
    ADD_DEFINITIONS(-DCCL_DLL)
ENDIF()

#==========================================================
# Subdirectories
#==========================================================

ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(commonOSG)
ADD_SUBDIRECTORY(commonS11n)
ADD_SUBDIRECTORY(hps)

ADD_SUBDIRECTORY(kernel)
#ADD_SUBDIRECTORY(osgSDL)

ADD_SUBDIRECTORY(pluginCameraMgrOSG)
ADD_SUBDIRECTORY(pluginCoordinateConversionGCC)
ADD_SUBDIRECTORY(pluginCoordinateConversionMgr)
ADD_SUBDIRECTORY(pluginCoordinateConversionTM)
ADD_SUBDIRECTORY(pluginDefFileReader)
ADD_SUBDIRECTORY(pluginEntityMgr)
#ADD_SUBDIRECTORY(pluginEnvRegionMgr)
IF(NOT MSVC)
    # Never been tested under MSVC
    ADD_SUBDIRECTORY(pluginEphemerisModel)
ENDIF()
ADD_SUBDIRECTORY(pluginExecLengthTiming)
#ADD_SUBDIRECTORY(pluginGlobalWeatherMgr)
ADD_SUBDIRECTORY(pluginMissionFuncsMgr)
ADD_SUBDIRECTORY(pluginMissionFuncsOSG)
#ADD_SUBDIRECTORY(pluginMotionTrackerJoystickSDL)
ADD_SUBDIRECTORY(pluginMotionTrackerMgr)
ADD_SUBDIRECTORY(pluginMotionTrackerRecvr)
ADD_SUBDIRECTORY(pluginRenderAmbientLightOSG)
#ADD_SUBDIRECTORY(pluginRenderAtmosphericOSG)
#ADD_SUBDIRECTORY(pluginRenderCameraCigiSDLOSG)
ADD_SUBDIRECTORY(pluginRenderDynamicLightOSG)
ADD_SUBDIRECTORY(pluginRenderEntsAutoArticulationOSG)
ADD_SUBDIRECTORY(pluginRenderEntsHighVisibilityOSG)
ADD_SUBDIRECTORY(pluginRenderEntsLabelsOSG)
ADD_SUBDIRECTORY(pluginRenderEntsModelFileOSG)
ADD_SUBDIRECTORY(pluginRenderEntsNearModel)
ADD_SUBDIRECTORY(pluginRenderEntsOSG)
ADD_SUBDIRECTORY(pluginRenderEntsParticleSysOSG)
ADD_SUBDIRECTORY(pluginRenderEntsScale)
#ADD_SUBDIRECTORY(pluginRenderEntsSoundOSGAL)
ADD_SUBDIRECTORY(pluginRenderEphemerisOSG)
ADD_SUBDIRECTORY(pluginRenderOSG)
ADD_SUBDIRECTORY(pluginRenderSkyOSG)
ADD_SUBDIRECTORY(pluginRenderSoundRootOSGAL)
ADD_SUBDIRECTORY(pluginRenderSplashScreenOSG)
ADD_SUBDIRECTORY(pluginRenderStatisticsOSG)
ADD_SUBDIRECTORY(pluginRenderSymbologyOSG)
ADD_SUBDIRECTORY(pluginRenderTerrainOSG)
ADD_SUBDIRECTORY(pluginRenderTV)
ADD_SUBDIRECTORY(pluginS11nEntities)
ADD_SUBDIRECTORY(pluginS11nRoot)
ADD_SUBDIRECTORY(pluginS11nSymbology)
ADD_SUBDIRECTORY(pluginS11nXML)
ADD_SUBDIRECTORY(pluginSymbologyMgr)
ADD_SUBDIRECTORY(pluginSyncedRandomNumbers)
ADD_SUBDIRECTORY(pluginTerrainMgr)
ADD_SUBDIRECTORY(pluginUserInputMgrOSGGA)
ADD_SUBDIRECTORY(pluginVideoCapture)
ADD_SUBDIRECTORY(pluginViewMgr)
ADD_SUBDIRECTORY(utils)

IF(EXISTS "${CMAKE_SOURCE_DIR}/proprietary" AND IS_DIRECTORY "${CMAKE_SOURCE_DIR}/proprietary")
    #Compile with proprietary plugins.
    ADD_SUBDIRECTORY(proprietary)
ENDIF()
