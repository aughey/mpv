MPV_PLUGIN_INIT(PluginRenderEntsHighVisibilityOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderEntsHighVisibilityOSG_PRIVATE_HDRS
    PluginRenderEntsHighVisibilityOSG.h
    SphereElement.h
    SphereElementFactory.h
    SphereIndicator.h
)
SET(PluginRenderEntsHighVisibilityOSG_SRCS
    PluginRenderEntsHighVisibilityOSG.cpp
    SphereElement.cpp
    SphereElementFactory.cpp
    SphereIndicator.cpp
)

ADD_LIBRARY(PluginRenderEntsHighVisibilityOSG MODULE
    ${PluginRenderEntsHighVisibilityOSG_PUBLIC_HDRS}
    ${PluginRenderEntsHighVisibilityOSG_PRIVATE_HDRS}
    ${PluginRenderEntsHighVisibilityOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsHighVisibilityOSG)

TARGET_LINK_LIBRARIES(PluginRenderEntsHighVisibilityOSG
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsHighVisibilityOSG
    ${OSG_LIBRARY})
