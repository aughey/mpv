MPV_PLUGIN_INIT(PluginRenderDynamicLightOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginRenderDynamicLightOSG_PRIVATE_HDRS
    PluginRenderDynamicLightOSG.h
)
SET(PluginRenderDynamicLightOSG_SRCS
    PluginRenderDynamicLightOSG.cpp
)

ADD_LIBRARY(PluginRenderDynamicLightOSG MODULE
    ${PluginRenderDynamicLightOSG_PUBLIC_HDRS}
    ${PluginRenderDynamicLightOSG_PRIVATE_HDRS}
    ${PluginRenderDynamicLightOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderDynamicLightOSG)

TARGET_LINK_LIBRARIES(PluginRenderDynamicLightOSG
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderDynamicLightOSG
    ${OSG_LIBRARY})
