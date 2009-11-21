MPV_PLUGIN_INIT(PluginRenderEntsLabelsOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderEntsLabelsOSG_PRIVATE_HDRS
    LabelElement.h
    LabelElementFactory.h
    LabelNode.h
    LabelNodeComponentImp.h
    PluginRenderEntsLabelsOSG.h
)
SET(PluginRenderEntsLabelsOSG_SRCS
    LabelElement.cpp
    LabelElementFactory.cpp
    LabelNode.cpp
    LabelNodeComponentImp.cpp
    PluginRenderEntsLabelsOSG.cpp
)

ADD_LIBRARY(PluginRenderEntsLabelsOSG MODULE
    ${PluginRenderEntsLabelsOSG_PUBLIC_HDRS}
    ${PluginRenderEntsLabelsOSG_PRIVATE_HDRS}
    ${PluginRenderEntsLabelsOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsLabelsOSG)

TARGET_LINK_LIBRARIES(PluginRenderEntsLabelsOSG
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsLabelsOSG
    ${OSGTEXT_LIBRARY} ${OSG_LIBRARY})
