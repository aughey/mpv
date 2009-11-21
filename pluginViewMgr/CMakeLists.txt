MPV_PLUGIN_INIT(PluginViewMgr)

SET(PluginViewMgr_PRIVATE_HDRS
    PluginViewMgr.h
    ViewCtrlP.h
    ViewDefP.h
)
SET(PluginViewMgr_SRCS
    PluginViewMgr.cpp
    ViewCtrlP.cpp
    ViewDefP.cpp
)

ADD_LIBRARY(PluginViewMgr MODULE
    ${PluginViewMgr_PUBLIC_HDRS}
    ${PluginViewMgr_PRIVATE_HDRS}
    ${PluginViewMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginViewMgr)

TARGET_LINK_LIBRARIES(PluginViewMgr mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginViewMgr)
