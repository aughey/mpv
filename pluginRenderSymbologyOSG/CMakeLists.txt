MPV_PLUGIN_INIT(PluginRenderSymbologyOSG)
INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(../commonOSG)

SET(PluginRenderSymbologyOSG_PRIVATE_HDRS
	PluginRenderSymbologyOSG.h
	SymbolCircleImpOSG.h
	SymbolLineImpOSG.h
	SymbolTextImpOSG.h
)
SET(PluginRenderSymbologyOSG_SRCS
	PluginRenderSymbologyOSG.cpp
	SymbolCircleImpOSG.cpp
	SymbolLineImpOSG.cpp
	SymbolTextImpOSG.cpp
)

ADD_LIBRARY(PluginRenderSymbologyOSG MODULE
	${PluginRenderSymbologyOSG_PUBLIC_HDRS}
	${PluginRenderSymbologyOSG_PRIVATE_HDRS}
	${PluginRenderSymbologyOSG_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginRenderSymbologyOSG)

TARGET_LINK_LIBRARIES(PluginRenderSymbologyOSG
	mpvcommon mpvcommonosg)
MPV_TARGET_LINK_OSG_LIBRARIES(PluginRenderSymbologyOSG
    ${OSGDB_LIBRARY} ${OSGTEXT_LIBRARY} ${OSG_LIBRARY})

