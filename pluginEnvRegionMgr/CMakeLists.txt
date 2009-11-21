MPV_PLUGIN_INIT(PluginEnvRegionMgr)

SET(PluginEnvRegionMgr_PRIVATE_HDRS
    EnvRegionMgr.h
    ProcCompCtrl.h
    ProcEnvRegionCtrl.h
    ProcShortCompCtrl.h
    ProcWeatherCtrl.h
)
SET(PluginEnvRegionMgr_SRCS
    EnvRegionMgr.cpp
    ProcCompCtrl.cpp
    ProcEnvRegionCtrl.cpp
    ProcShortCompCtrl.cpp
    ProcWeatherCtrl.cpp
)

ADD_LIBRARY(PluginEnvRegionMgr MODULE
    ${PluginEnvRegionMgr_PUBLIC_HDRS}
    ${PluginEnvRegionMgr_PRIVATE_HDRS}
    ${PluginEnvRegionMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginEnvRegionMgr)

TARGET_LINK_LIBRARIES(PluginEnvRegionMgr mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginEnvRegionMgr)
