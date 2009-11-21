MPV_PLUGIN_INIT(PluginExecLengthTiming)

SET(PluginExecLengthTiming_PRIVATE_HDRS
    ExecLengthTiming.h
)
SET(PluginExecLengthTiming_SRCS
    ExecLengthTiming.cpp
)

ADD_LIBRARY(PluginExecLengthTiming MODULE
    ${PluginExecLengthTiming_PUBLIC_HDRS}
    ${PluginExecLengthTiming_PRIVATE_HDRS}
    ${PluginExecLengthTiming_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginExecLengthTiming)

TARGET_LINK_LIBRARIES(PluginExecLengthTiming mpvcommon)
