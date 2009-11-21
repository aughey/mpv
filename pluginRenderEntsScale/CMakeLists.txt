MPV_PLUGIN_INIT(PluginRenderEntsScale)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderEntsScale_PRIVATE_HDRS
    PluginRenderEntsScale.h
    ScaleComponentImp.h
    ScaleElement.h
    ScaleElementFactory.h
)
SET(PluginRenderEntsScale_SRCS
    PluginRenderEntsScale.cpp
    ScaleComponentImp.cpp
    ScaleElement.cpp
    ScaleElementFactory.cpp
)

ADD_LIBRARY(PluginRenderEntsScale MODULE
    ${PluginRenderEntsScale_PUBLIC_HDRS}
    ${PluginRenderEntsScale_PRIVATE_HDRS}
    ${PluginRenderEntsScale_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsScale)

TARGET_LINK_LIBRARIES(PluginRenderEntsScale
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsScale
    ${OSG_LIBRARY})
