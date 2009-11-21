MPV_PLUGIN_INIT(PluginRenderEphemerisOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderEphemerisOSG_PRIVATE_HDRS
	FollowEyePointTransform.h
	MoonNode.h
	PluginRenderEphemerisOSG.h
	SkyDomeNode.h
	SkyParameters.h
	StarCatalog.h
	StarDomeNode.h
	stars.h
)
SET(PluginRenderEphemerisOSG_SRCS
	FollowEyePointTransform.cpp
	MoonNode.cpp
	PluginRenderEphemerisOSG.cpp
	SkyDomeNode.cpp
	StarCatalog.cpp
	StarDomeNode.cpp
)

ADD_LIBRARY(PluginRenderEphemerisOSG MODULE
    ${PluginRenderEphemerisOSG_PUBLIC_HDRS}
    ${PluginRenderEphemerisOSG_PRIVATE_HDRS}
    ${PluginRenderEphemerisOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEphemerisOSG)

TARGET_LINK_LIBRARIES(PluginRenderEphemerisOSG
    mpvcommon)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEphemerisOSG
    ${OSG_LIBRARY} ${OSGDB_LIBRARY})

