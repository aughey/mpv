MPV_PLUGIN_INIT(PluginS11nSymbology)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/commonS11n)

SET(PluginS11nSymbology_PRIVATE_HDRS
	PluginS11nSymbology.h
	SymbolCircleImpS11n.h
	SymbolLineImpS11n.h
	SymbolTextImpS11n.h
)
SET(PluginS11nSymbology_SRCS
	PluginS11nSymbology.cpp
	SymbolCircleImpS11n.cpp
	SymbolLineImpS11n.cpp
	SymbolTextImpS11n.cpp
)

ADD_LIBRARY(PluginS11nSymbology MODULE
	${PluginS11nSymbology_PUBLIC_HDRS}
	${PluginS11nSymbology_PRIVATE_HDRS}
	${PluginS11nSymbology_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginS11nSymbology)

TARGET_LINK_LIBRARIES(PluginS11nSymbology mpvcommon mpvcommons11n)

