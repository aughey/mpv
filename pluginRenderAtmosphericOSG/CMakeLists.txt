MPV_PLUGIN_INIT(PluginRenderAtmosphericOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginRenderAtmosphericOSG_PRIVATE_HDRS
    PluginRenderAtmosphericOSG.h
)
SET(PluginRenderAtmosphericOSG_SRCS
    PluginRenderAtmosphericOSG.cpp
)

ADD_LIBRARY(PluginRenderAtmosphericOSG MODULE
    ${PluginRenderAtmosphericOSG_PUBLIC_HDRS}
    ${PluginRenderAtmosphericOSG_PRIVATE_HDRS}
    ${PluginRenderAtmosphericOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderAtmosphericOSG)

TARGET_LINK_LIBRARIES(PluginRenderAtmosphericOSG
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderAtmosphericOSG
    ${OSG_LIBRARY})

