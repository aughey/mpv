MPV_PLUGIN_INIT(PluginGlobalWeatherMgr)

SET(PluginGlobalWeatherMgr_PRIVATE_HDRS
    GlobalWeatherMgr.h
    ProcAtmosphereCtrl.h
    ProcCompCtrl.h
    ProcShortCompCtrl.h
    ProcWeatherCtrl.h
)
SET(PluginGlobalWeatherMgr_SRCS
    GlobalWeatherMgr.cpp
)

ADD_LIBRARY(PluginGlobalWeatherMgr MODULE
    ${PluginGlobalWeatherMgr_PUBLIC_HDRS}
    ${PluginGlobalWeatherMgr_PRIVATE_HDRS}
    ${PluginGlobalWeatherMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginGlobalWeatherMgr)

TARGET_LINK_LIBRARIES(PluginGlobalWeatherMgr mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginGlobalWeatherMgr)
