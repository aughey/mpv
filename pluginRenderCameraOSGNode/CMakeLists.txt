MPV_PLUGIN_INIT(PluginRenderCameraCigiSDLOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../osgSDL/include)
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderCameraCigiSDLOSG_PRIVATE_HDRS
	PluginRenderCameraOSG.h
	ViewportWrapper.h
)
SET(PluginRenderCameraCigiSDLOSG_SRCS
	PluginRenderCameraOSG.cpp
	ViewportWrapper.cpp
)

ADD_LIBRARY(PluginRenderCameraCigiSDLOSG MODULE
    ${PluginRenderCameraCigiSDLOSG_PUBLIC_HDRS}
    ${PluginRenderCameraCigiSDLOSG_PRIVATE_HDRS}
    ${PluginRenderCameraCigiSDLOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderCameraCigiSDLOSG)

TARGET_LINK_LIBRARIES(PluginRenderCameraCigiSDLOSG
	mpvcommon 
	mpvcommonosg
	${SDL_LIBRARY} osgSDL)

