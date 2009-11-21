MPV_PLUGIN_INIT(PluginRenderEntsParticleSysOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)
INCLUDE_DIRECTORIES(../hps)

SET(PluginRenderEntsParticleSysOSG_PRIVATE_HDRS
    ParticleSysAnimationImp.h
    ParticleSysElement.h
    ParticleSysElementFactory.h
    PluginRenderParticleSysOSG.h
)
SET(PluginRenderEntsParticleSysOSG_SRCS
    ParticleSysAnimationImp.cpp
    ParticleSysElement.cpp
    ParticleSysElementFactory.cpp
    PluginRenderParticleSysOSG.cpp
)

ADD_LIBRARY(PluginRenderEntsParticleSysOSG MODULE
    ${PluginRenderEntsParticleSysOSG_PUBLIC_HDRS}
    ${PluginRenderEntsParticleSysOSG_PRIVATE_HDRS}
    ${PluginRenderEntsParticleSysOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsParticleSysOSG)

TARGET_LINK_LIBRARIES(PluginRenderEntsParticleSysOSG
    mpvcommon mpvcommonosg mpvhps)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsParticleSysOSG
    ${OSG_LIBRARY})
