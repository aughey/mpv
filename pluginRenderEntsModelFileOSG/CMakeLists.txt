MPV_PLUGIN_INIT(PluginRenderEntsModelFileOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderEntsModelFileOSG_PRIVATE_HDRS
	ModelCache.h
	ModelElement.h
	ModelElementFactory.h
	PluginRenderEntsModelFileOSG.h
	SequenceNodeAnimationImp.h
)
SET(PluginRenderEntsModelFileOSG_SRCS
	ModelCache.cpp
	ModelElement.cpp
	ModelElementFactory.cpp
	PluginRenderEntsModelFileOSG.cpp
	SequenceNodeAnimationImp.cpp
)

ADD_LIBRARY(PluginRenderEntsModelFileOSG MODULE
    ${PluginRenderEntsModelFileOSG_PUBLIC_HDRS}
    ${PluginRenderEntsModelFileOSG_PRIVATE_HDRS}
    ${PluginRenderEntsModelFileOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsModelFileOSG)

TARGET_LINK_LIBRARIES(PluginRenderEntsModelFileOSG
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsModelFileOSG
        ${OSGDB_LIBRARY} ${OSGSIM_LIBRARY} ${OSG_LIBRARY})
