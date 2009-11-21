MPV_PLUGIN_INIT(PluginRenderTerrainOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderTerrainOSG_PRIVATE_HDRS
    MoveDummyTerrainTransform.h
    PluginRenderTerrainOSG.h
    TerrainImpOSG.h
)
SET(PluginRenderTerrainOSG_SRCS
    MoveDummyTerrainTransform.cpp
    PluginRenderTerrainOSG.cpp
    TerrainImpOSG.cpp
)

ADD_LIBRARY(PluginRenderTerrainOSG MODULE
    ${PluginRenderTerrainOSG_PUBLIC_HDRS}
    ${PluginRenderTerrainOSG_PRIVATE_HDRS}
    ${PluginRenderTerrainOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderTerrainOSG)

TARGET_LINK_LIBRARIES(PluginRenderTerrainOSG
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderTerrainOSG
    ${OSGDB_LIBRARY} ${OSGUTIL_LIBRARY} ${OSG_LIBRARY})
TARGET_LINK_LIBRARIES(PluginRenderTerrainOSG
    optimized ${OPENTHREADS_LIBRARY} debug ${OPENTHREADS_LIBRARY_DEBUG})

