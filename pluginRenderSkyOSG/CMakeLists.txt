MPV_PLUGIN_INIT(PluginRenderSkyOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderSkyOSG_PRIVATE_HDRS
    MoveSkyTransform.h
    PluginRenderSkyOSG.h
    SkyDomeFileNode.h
    SkyDomeNode.h
)
SET(PluginRenderSkyOSG_SRCS
    MoveSkyTransform.cpp
    PluginRenderSkyOSG.cpp
    SkyDomeFileNode.cpp
    SkyDomeNode.cpp
)

ADD_LIBRARY(PluginRenderSkyOSG MODULE
    ${PluginRenderSkyOSG_PUBLIC_HDRS}
    ${PluginRenderSkyOSG_PRIVATE_HDRS}
    ${PluginRenderSkyOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderSkyOSG)

TARGET_LINK_LIBRARIES(PluginRenderSkyOSG
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderSkyOSG
    ${OSGDB_LIBRARY}
    ${OSGUTIL_LIBRARY}
    ${OSG_LIBRARY})

