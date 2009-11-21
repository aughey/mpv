MPV_PLUGIN_INIT(PluginMotionTrackerRecvr)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginMotionTrackerRecvr_PRIVATE_HDRS
    PluginMotionTrackerRecvr.h
)
SET(PluginMotionTrackerRecvr_SRCS
    PluginMotionTrackerRecvr.cpp
)

ADD_LIBRARY(PluginMotionTrackerRecvr MODULE
    ${PluginMotionTrackerRecvr_PUBLIC_HDRS}
    ${PluginMotionTrackerRecvr_PRIVATE_HDRS}
    ${PluginMotionTrackerRecvr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginMotionTrackerRecvr)

TARGET_LINK_LIBRARIES(PluginMotionTrackerRecvr mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginMotionTrackerRecvr ${OSG_LIBRARY})
