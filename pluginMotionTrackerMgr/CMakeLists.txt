MPV_PLUGIN_INIT(PluginMotionTrackerMgr)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginMotionTrackerMgr_PRIVATE_HDRS
    MotionTrackerCtrlP.h
    PluginMotionTrackerMgr.h
    PositionRequestP.h
)
SET(PluginMotionTrackerMgr_SRCS
    MotionTrackerCtrlP.cpp
    PluginMotionTrackerMgr.cpp
    PositionRequestP.cpp
)

ADD_LIBRARY(PluginMotionTrackerMgr MODULE
    ${PluginMotionTrackerMgr_PUBLIC_HDRS}
    ${PluginMotionTrackerMgr_PRIVATE_HDRS}
    ${PluginMotionTrackerMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginMotionTrackerMgr)

TARGET_LINK_LIBRARIES(PluginMotionTrackerMgr mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginMotionTrackerMgr ${OSG_LIBRARY})
