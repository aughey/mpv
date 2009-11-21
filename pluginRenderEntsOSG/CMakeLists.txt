MPV_PLUGIN_INIT(PluginRenderEntsOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderEntsOSG_PRIVATE_HDRS
    EntityNodeFactory.h
    GroupElement.h
    GroupElementFactory.h
    SwitchElement.h
    SwitchElementFactory.h
    TransformElement.h
    TransformElementFactory.h
    PluginRenderEntitiesOSG.h
)
SET(PluginRenderEntsOSG_SRCS
    EntityNodeFactory.cpp
    GroupElement.cpp
    GroupElementFactory.cpp
    PluginRenderEntitiesOSG.cpp
    SwitchElement.cpp
    SwitchElementFactory.cpp
    TransformElement.cpp
    TransformElementFactory.cpp
)

ADD_LIBRARY(PluginRenderEntsOSG MODULE
    ${PluginRenderEntsOSG_PUBLIC_HDRS}
    ${PluginRenderEntsOSG_PRIVATE_HDRS}
    ${PluginRenderEntsOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsOSG)

TARGET_LINK_LIBRARIES(PluginRenderEntsOSG
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsOSG
    ${OSG_LIBRARY})
