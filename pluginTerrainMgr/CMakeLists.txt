MPV_PLUGIN_INIT(PluginTerrainMgr)

SET(PluginTerrainMgr_PRIVATE_HDRS
    PluginTerrainMgr.h
)
SET(PluginTerrainMgr_SRCS
    PluginTerrainMgr.cpp
)

ADD_LIBRARY(PluginTerrainMgr MODULE
    ${PluginTerrainMgr_PUBLIC_HDRS}
    ${PluginTerrainMgr_PRIVATE_HDRS}
    ${PluginTerrainMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginTerrainMgr)

TARGET_LINK_LIBRARIES(PluginTerrainMgr mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginTerrainMgr)
