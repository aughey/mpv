MPV_PLUGIN_INIT(PluginSymbologyMgr)

SET(PluginSymbologyMgr_PRIVATE_HDRS
	PluginSymbologyMgr.h
)
SET(PluginSymbologyMgr_SRCS
	PluginSymbologyMgr.cpp
)

ADD_LIBRARY(PluginSymbologyMgr MODULE
	${PluginSymbologyMgr_PUBLIC_HDRS}
	${PluginSymbologyMgr_PRIVATE_HDRS}
	${PluginSymbologyMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginSymbologyMgr)

TARGET_LINK_LIBRARIES(PluginSymbologyMgr mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginSymbologyMgr)
