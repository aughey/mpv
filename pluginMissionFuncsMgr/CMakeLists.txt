MPV_PLUGIN_INIT(PluginMissionFuncsMgr)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginMissionFuncsMgr_PRIVATE_HDRS
	HOATDispatcher.h
	LOSDispatcher.h
	PluginMissionFuncsMgr.h
)
SET(PluginMissionFuncsMgr_SRCS
	HOATDispatcher.cpp
	LOSDispatcher.cpp
	PluginMissionFuncsMgr.cpp
)

ADD_LIBRARY(PluginMissionFuncsMgr MODULE
    ${PluginMissionFuncsMgr_PUBLIC_HDRS}
    ${PluginMissionFuncsMgr_PRIVATE_HDRS}
    ${PluginMissionFuncsMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginMissionFuncsMgr)

TARGET_LINK_LIBRARIES(PluginMissionFuncsMgr mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginMissionFuncsMgr
    ${OSGUTIL_LIBRARY} ${OSG_LIBRARY})
