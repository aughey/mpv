MPV_PLUGIN_INIT(PluginRenderEntsAutoArticulationOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/commonOSG)

SET(PluginRenderEntsAutoArticulationOSG_PRIVATE_HDRS
	AutoArticulationElement.h
	AutoArticulationElementFactory.h
	PluginRenderEntsAutoArticulationOSG.h
)
SET(PluginRenderEntsAutoArticulationOSG_SRCS
	AutoArticulationElement.cpp
	AutoArticulationElementFactory.cpp
	PluginRenderEntsAutoArticulationOSG.cpp
)

ADD_LIBRARY(PluginRenderEntsAutoArticulationOSG MODULE
    ${PluginRenderEntsAutoArticulationOSG_PUBLIC_HDRS}
    ${PluginRenderEntsAutoArticulationOSG_PRIVATE_HDRS}
    ${PluginRenderEntsAutoArticulationOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderEntsAutoArticulationOSG)

TARGET_LINK_LIBRARIES(PluginRenderEntsAutoArticulationOSG
    mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderEntsAutoArticulationOSG
    ${OSG_LIBRARY})
