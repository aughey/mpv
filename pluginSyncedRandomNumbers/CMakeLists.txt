MPV_PLUGIN_INIT(PluginSyncedRandomNumbers)

SET(PluginSyncedRandomNumbers_PRIVATE_HDRS
    IGCtrlP.h
    PluginSyncedRandomNumbers.h
    RandGenerateID.h
)
SET(PluginSyncedRandomNumbers_SRCS
    IGCtrlP.cpp
    PluginSyncedRandomNumbers.cpp
)

ADD_LIBRARY(PluginSyncedRandomNumbers MODULE
    ${PluginSyncedRandomNumbers_PUBLIC_HDRS}
    ${PluginSyncedRandomNumbers_PRIVATE_HDRS}
    ${PluginSyncedRandomNumbers_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginSyncedRandomNumbers)

TARGET_LINK_LIBRARIES(PluginSyncedRandomNumbers mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginSyncedRandomNumbers)
