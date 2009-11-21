MPV_PLUGIN_INIT(PluginCameraMgrOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})

SET(PluginCameraMgrOSG_PRIVATE_HDRS
    JoystickBindings.h
    PluginCameraMgrOSG.h
)
SET(PluginCameraMgrOSG_SRCS
    JoystickBindings.cpp
    PluginCameraMgrOSG.cpp
)

ADD_LIBRARY(PluginCameraMgrOSG MODULE
    ${PluginCameraMgrOSG_PUBLIC_HDRS}
    ${PluginCameraMgrOSG_PRIVATE_HDRS}
    ${PluginCameraMgrOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginCameraMgrOSG)

TARGET_LINK_LIBRARIES(PluginCameraMgrOSG mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginCameraMgrOSG
    ${OSGGA_LIBRARY} ${OSG_LIBRARY})
