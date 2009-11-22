MPV_PLUGIN_INIT(PluginRenderCameraOSGNode)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderCameraOSGNode_PRIVATE_HDRS
	PluginRenderCameraOSGNode.h
	ViewportWrapper.h
)
SET(PluginRenderCameraOSGNode_SRCS
	PluginRenderCameraOSGNode.cpp
	ViewportWrapper.cpp
)

ADD_LIBRARY(PluginRenderCameraOSGNode MODULE
    ${PluginRenderCameraOSGNode_PUBLIC_HDRS}
    ${PluginRenderCameraOSGNode_PRIVATE_HDRS}
    ${PluginRenderCameraOSGNode_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderCameraOSGNode)

TARGET_LINK_LIBRARIES(PluginRenderCameraOSGNode
	mpvcommon 
	mpvcommonosg)
