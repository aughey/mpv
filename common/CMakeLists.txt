INCLUDE_DIRECTORIES(${CCL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PDL_INCLUDE_DIRS})

IF(MSVC)
    ADD_DEFINITIONS(-DYY_NO_UNISTD_H)
ENDIF()

SET(common_PUBLIC_HDRS
    AllCigi.h
    Animation.h
    AnimationContainer.h
    Articulation.h
    ArticulationContainer.h
    BindSlot.h
    Blackboard.h
    Component.h
    ComponentContainer.h
    CoordSet.h
    CoordSysParams.h
    CoordinateConversionObserver.h
    CoordinateConverter.h
    DefFileAttrib.h
    DefFileGroup.h
    DefFileParser.h
    Entity.h
    EntityContainer.h
#    EnvRegion.h
    ExtrapolationSet.h
    GenerateID.h
    GeodeticObject.h
#    GlobalWeather.h
    HOTRequest.h
    HOTResponse.h
    LicenseInfo.h
    LOSRequest.h
    LOSResponse.h
    MissionFunctionsWorker.h
    MPVCommonTypes.h
    MPVExceptions.h
    MPVLight.h
    MPVTimer.h
    Mtx3.h
    Mtx4.h
    Network.h
    Plugin.h
    Referenced.h
    RefPtr.h
	SimpleTimer.h
	SimpleTimerBase.h
	SimpleTimerGTOD.h
	SimpleTimerLinuxX86.h
	SimpleTimerNoOp.h
	SimpleTimerPAPI.h
	SimpleTimerWindows.h
    StateContext.h
    Symbol.h
    SymbolCircle.h
	SymbolContainer.h
    SymbolLine.h
    SymbolText.h
    SymbolSurface.h
    SymbolSurfaceContainer.h
    SystemState.h
    Terrain.h
    TerrainContainer.h
    TrackerParams.h
    Vect2.h
    Vect3.h
    Vect4.h
    View.h
    WeatherCtrl.h
    WeatherCtrlList.h
)
SET(common_PRIVATE_HDRS
    deffile-yacc.hpp
    )
SET(common_SRCS
    Animation.cpp
    AnimationContainer.cpp
    Articulation.cpp
    ArticulationContainer.cpp
    Blackboard.cpp
    Component.cpp
    ComponentContainer.cpp
    CoordSet.cpp
    CoordinateConversionObserver.cpp
    CoordinateConverter.cpp
    DefFileAttrib.cpp
    DefFileGroup.cpp
    DefFileParser.cpp
    deffile-lex.cpp
    deffile-yacc.cpp
    Entity.cpp
    EntityContainer.cpp
#    EnvRegion.cpp
    GeodeticObject.cpp
#    GlobalWeather.cpp
    HOTRequest.cpp
    HOTResponse.cpp
    LicenseInfo.cpp
    LOSRequest.cpp
    LOSResponse.cpp
    MissionFunctionsWorker.cpp
    MPVExceptions.cpp
    MPVTimer.cpp
    Mtx3.cpp
    Mtx4.cpp
    Network.cpp
    Plugin.cpp
    Referenced.cpp
    StateContext.cpp
    Symbol.cpp
    SymbolCircle.cpp
	SymbolContainer.cpp
    SymbolLine.cpp
    SymbolText.cpp
    SymbolSurface.cpp
    SymbolSurfaceContainer.cpp
    Terrain.cpp
    TerrainContainer.cpp
    Vect2.cpp
    Vect3.cpp
    Vect4.cpp
    View.cpp
    WeatherCtrl.cpp
)

#==========================================================
# Conditional Additions to libcommon
#==========================================================

#
# Timer classes are conditionally added to the build, based on the platform.
#

# Windows high-perf timer
IF(WIN32)
	SET(common_SRCS ${common_SRCS} SimpleTimerWindows.cpp)
ENDIF(WIN32)

# PAPI timer
IF(USE_PAPI)
	SET(common_SRCS ${common_SRCS} SimpleTimerPAPI.cpp)
ENDIF(USE_PAPI)

# Linux x86 TSC timer
IF(UNIX)
	IF( CMAKE_COMPILER_IS_GNUCXX AND CMAKE_SYSTEM_NAME STREQUAL Linux )
		SET(common_SRCS ${common_SRCS} SimpleTimerLinuxX86.cpp)
	ENDIF()
ENDIF(UNIX)

# GetTimeOfDay timer
# fixme - Unix systems have gettimeofday(); who else has it?
IF(UNIX)
	SET(common_SRCS ${common_SRCS} SimpleTimerGTOD.cpp)
ENDIF(UNIX)

#==========================================================
# Create the library.
#==========================================================

ADD_LIBRARY(mpvcommon SHARED
    ${common_PUBLIC_HDRS} ${common_PRIVATE_HDRS} ${common_SRCS})
SET_TARGET_PROPERTIES(mpvcommon PROPERTIES DEFINE_SYMBOL _EXPORT_MPVCMN_)
    
IF(CMAKE_MAJOR_VERSION EQUAL 2 AND
   CMAKE_MINOR_VERSION LESS 6)
   MPV_COPY_DLL_OUTPUT_TO_BINARY_DIR(mpvcommon)
ENDIF()

#==========================================================
# Link dependencies.
#==========================================================

IF(CMAKE_MAJOR_VERSION GREATER 1 AND
   CMAKE_MINOR_VERSION GREATER 4)
# On Boost >= 2.6.0 rely on the COMPONENTS feature in
# CMake 2.6.0's FindBoost module (see call to FIND_PACKAGE
# in toplevel CMakeLists.txt)
    TARGET_LINK_LIBRARIES(mpvcommon ${Boost_LIBRARIES})
ELSE()
    # For backwards compatability with CMake 2.4.x (and the old FindBoost)
    TARGET_LINK_LIBRARIES(mpvcommon boost_signals)
ENDIF()

MPV_TARGET_LINK_CCL_LIBRARIES(mpvcommon)

IF(WIN32)
    TARGET_LINK_LIBRARIES(mpvcommon Ws2_32.lib)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(mpvcommon ${PDL_LIBRARIES})

#==========================================================
# Install rule
#==========================================================

INSTALL(TARGETS mpvcommon
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
