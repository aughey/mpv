MPV_PLUGIN_INIT(PluginRenderAmbientLightOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginRenderAmbientLightOSG_PRIVATE_HDRS
    PluginRenderAmbientLightOSG.h
)
SET(PluginRenderAmbientLightOSG_SRCS
    PluginRenderAmbientLightOSG.cpp
)

ADD_LIBRARY(PluginRenderAmbientLightOSG MODULE
    ${PluginRenderAmbientLightOSG_PUBLIC_HDRS}
    ${PluginRenderAmbientLightOSG_PRIVATE_HDRS}
    ${PluginRenderAmbientLightOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderAmbientLightOSG)

TARGET_LINK_LIBRARIES(PluginRenderAmbientLightOSG
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderAmbientLightOSG ${OSG_LIBRARY})
