MPV_PLUGIN_INIT(PluginEntityMgr)

SET(PluginEntityMgr_PRIVATE_HDRS
    AnimStopNotificationListener.h
    EnhancedEntityContainer.h
    EntityCoordinateConversionObserver.h
    EntityFactory.h
    PluginEntityMgr.h
    ProcArtPart.h
    ProcCompCtrl.h
    ProcEntityCtrl.h
#    ProcPositionReq.h
#    ProcRateCtrl.h
    ProcShortArtPart.h
    ProcShortCompCtrl.h
#    ProcTrajectory.h
#    ProcWeatherCtrl.h
)
SET(PluginEntityMgr_SRCS
    AnimStopNotificationListener.cpp
    EnhancedEntityContainer.cpp
    EntityCoordinateConversionObserver.cpp
    EntityFactory.cpp
    PluginEntityMgr.cpp
    ProcArtPart.cpp
    ProcCompCtrl.cpp
    ProcEntityCtrl.cpp
#    ProcPositionReq.cpp
#    ProcRateCtrl.cpp
    ProcShortArtPart.cpp
    ProcShortCompCtrl.cpp
#    ProcTrajectory.cpp
#    ProcWeatherCtrl.cpp
)

ADD_LIBRARY(PluginEntityMgr MODULE
    ${PluginEntityMgr_PUBLIC_HDRS}
    ${PluginEntityMgr_PRIVATE_HDRS}
    ${PluginEntityMgr_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginEntityMgr)

TARGET_LINK_LIBRARIES(PluginEntityMgr mpvcommon)
MPV_TARGET_LINK_CCL_LIBRARIES(PluginEntityMgr)
