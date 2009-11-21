MPV_PLUGIN_INIT(PluginMissionFuncsOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginMissionFuncsOSG_PRIVATE_HDRS
    HOATHandler.h
    IntersectionFinder.h
    LOSHandler.h
    OsgIntersectionFinder.h
    OsgLineDrawer.h
    PluginMissionFuncsOSG.h
)
SET(PluginMissionFuncsOSG_SRCS
    HOATHandler.cpp
    LOSHandler.cpp
    OsgIntersectionFinder.cc
    OsgLineDrawer.cc
    PluginMissionFuncsOSG.cpp
)

ADD_LIBRARY(PluginMissionFuncsOSG MODULE
    ${PluginMissionFuncsOSG_PUBLIC_HDRS}
    ${PluginMissionFuncsOSG_PRIVATE_HDRS}
    ${PluginMissionFuncsOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginMissionFuncsOSG)

TARGET_LINK_LIBRARIES(PluginMissionFuncsOSG mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginMissionFuncsOSG
    ${OSGUTIL_LIBRARY} ${OSG_LIBRARY})
