# - Find the CIGI Class Library
# This module searches for the CIGI Class Library.  It should be
# invoked by calling: FIND_PACKAGE(CCL).
#
# This module defines these variables:
#  CCL_DEFINITIONS - Preprocessor definitions required to use the CIGI Class
#                    Library.  This is either CIGI_BIG_ENDIAN or
#                    CIGI_LITTLE_ENDIAN.  Use
#                    ADD_DEFINITIONS(${CCL_DEFINITIONS})
#  CCL_FOUND - True if the CIGI Class Library was found
#  CCL_LIBRARY - The location of the CIGI Class Library
#  CCL_LIBRARY_DEBUG - The location of the CIGI Class Debug Library when using
#                      MSVC to compile.
#  CCL_INCLUDE_DIR - The location of the CIGI Class Library header files
#
# This module accepts these variables:
#
#  CCL_ROOT (environment variable) - Provide the exact CCL prefix to use
#  CCL_ROOT (CMake variable) - Provide the exact CCL prefix to use



#
# pkg-config, if available, can potentially provide us with the location of 
# the CCL headers and libraries.  The cmake pkg-config macros will define 
# CCL_PKGCONFIG_INCLUDE_DIRS, CCL_PKGCONFIG_LIBRARY_DIRS, and a few other 
# variables if the ccl is found.
FIND_PACKAGE(PkgConfig)
IF(PKG_CONFIG_FOUND)
	pkg_check_modules( CCL_PKGCONFIG cigicl>=3.3.0 )
ENDIF(PKG_CONFIG_FOUND)

INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(CCL_is_big_endian)

IF(CCL_is_big_endian)
    SET(CCL_DEFINITIONS -DCIGI_BIG_ENDIAN)
ELSE(CCL_is_big_endian)
    SET(CCL_DEFINITIONS -DCIGI_LITTLE_ENDIAN)
ENDIF(CCL_is_big_endian)

#
# CCL Include Path
#

FIND_PATH(CCL_INCLUDE_DIR CigiTypes.h
    PATHS
    $ENV{CCL_ROOT}/include/cigicl
    ${CCL_ROOT}/include/cigicl
    $ENV{CCL_ROOT}/include
    ${CCL_ROOT}/include
	${CCL_PKGCONFIG_INCLUDE_DIRS}
    DOC "The CIGI Class Library (CCL) include path"
    NO_DEFAULT_PATH
)
FIND_PATH(CCL_INCLUDE_DIR CigiTypes.h
    PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /sw/include # Fink
    /opt/local/include# DarwinPorts
    /opt/csw/include# Blastwave
    /opt/include
    /usr/include
    /usr/local/include

    PATH_SUFFIXES cigicl
    DOC "The CIGI Class Library (CCL) include path"
)

#
# CCL
#
        
FIND_LIBRARY(CCL_LIBRARY NAMES cigicl ccl_dll
    PATHS
        $ENV{CCL_ROOT}/lib
        ${CCL_ROOT}/lib
        # Facilitate the CCL's shipped VCproj files
        $ENV{CCL_ROOT}/bin/dllReleaseMT_DLL
        ${CCL_ROOT}/bin/dllReleaseMT_DLL
        # Facilitate pkgconfig located libraries
        ${CCL_PKGCONFIG_LIBRARY_DIRS}
    DOC "The CIGI Class Library (CCL) library file"
    NO_DEFAULT_PATH
)
FIND_LIBRARY(CCL_LIBRARY NAMES cigicl ccl_dll
    ~/Library/Frameworks
    /Library/Frameworks
    /sw
    /opt/local
    /opt/csw
    /opt
    /usr
    /usr/local

    PATH_SUFFIXES lib64 lib
    DOC "The CIGI Class Library (CCL) library file"
)

#
# CCL Debug
# Note: this is only searched for when using MSVC because that
# compiler requires linking debug targets against debug libraries).
#
# For other platforms simply ensure that CCL_LIBRARY is pointed to a debug
# library at build time if you need to debug into the CCL for any reason.
#

IF(MSVC)
    FIND_LIBRARY(CCL_LIBRARY_DEBUG NAMES cigicl_debug ccl_dll
        PATHS
            $ENV{CCL_ROOT}/lib
            ${CCL_ROOT}/lib
            # Facilitate the CCL's shipped VCproj files
            $ENV{CCL_ROOT}/bin/dllDebugMT_DLL
            ${CCL_ROOT}/bin/dllDebugMT_DLL
            # Facilitate pkgconfig located libraries
            ${CCL_PKGCONFIG_LIBRARY_DIRS}
        DOC "A Debugged build of the CIGI Class Library (CCL)"
        NO_DEFAULT_PATH
    )
    FIND_LIBRARY(CCL_LIBRARY_DEBUG NAMES cigicl_debug ccl_dll
        DOC "A Debugged build of the CIGI Class Library (CCL)"
    )
ENDIF(MSVC)

SET(CCL_FOUND "NO")
IF(CCL_LIBRARY AND CCL_INCLUDE_DIR)
    SET(CCL_FOUND "YES")
    SET(CCL_LIBRARIES    ${CCL_LIBRARY})
    SET(CCL_INCLUDE_DIRS ${CCL_INCLUDE_DIR})
ENDIF(CCL_LIBRARY AND CCL_INCLUDE_DIR)

IF(CCL_FOUND)
    IF(NOT CCL_FIND_QUIETLY)
        MESSAGE(STATUS "Found CCL: ${CCL_LIBRARIES}")
    ENDIF(NOT CCL_FIND_QUIETLY)
ELSE(CCL_FOUND) 
    IF(CCL_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find CIGI Class Library (CCL)")
    ENDIF(CCL_FIND_REQUIRED)
ENDIF(CCL_FOUND)
