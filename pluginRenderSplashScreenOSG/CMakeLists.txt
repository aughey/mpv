MPV_PLUGIN_INIT(PluginRenderSplashScreenOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderSplashScreenOSG_PRIVATE_HDRS
    PluginRenderSplashScreenOSG.h
)
SET(PluginRenderSplashScreenOSG_SRCS
    PluginRenderSplashScreenOSG.cpp
)

ADD_LIBRARY(PluginRenderSplashScreenOSG MODULE
    ${PluginRenderSplashScreenOSG_PUBLIC_HDRS}
    ${PluginRenderSplashScreenOSG_PRIVATE_HDRS}
    ${PluginRenderSplashScreenOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderSplashScreenOSG)

TARGET_LINK_LIBRARIES(PluginRenderSplashScreenOSG
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderSplashScreenOSG
    ${OSGDB_LIBRARY} ${OSGTEXT_LIBRARY} ${OSG_LIBRARY})

