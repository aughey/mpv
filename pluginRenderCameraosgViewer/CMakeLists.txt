MPV_PLUGIN_INIT(PluginRenderCameraosgViewer)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderCameraosgViewer_PRIVATE_HDRS
	PluginRenderCameraosgViewer.h
)
SET(PluginRenderCameraosgViewer_SRCS
	PluginRenderCameraosgViewer.cpp
)

ADD_LIBRARY(PluginRenderCameraosgViewer MODULE
    ${PluginRenderCameraosgViewer_PUBLIC_HDRS}
    ${PluginRenderCameraosgViewer_PRIVATE_HDRS}
    ${PluginRenderCameraosgViewer_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderCameraosgViewer)

TARGET_LINK_LIBRARIES(PluginRenderCameraosgViewer
	mpvcommon 
	mpvcommonosg)
