MPV_PLUGIN_INIT(PluginS11nEntities)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/commonS11n)

SET(PluginS11nEntities_PRIVATE_HDRS
	EntityImpS11n.h
	PluginS11nEntities.h
)
SET(PluginS11nEntities_SRCS
	EntityImpS11n.cpp
	PluginS11nEntities.cpp
)

ADD_LIBRARY(PluginS11nEntities MODULE
	${PluginS11nEntities_PUBLIC_HDRS}
	${PluginS11nEntities_PRIVATE_HDRS}
	${PluginS11nEntities_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginS11nEntities)

TARGET_LINK_LIBRARIES(PluginS11nEntities mpvcommon mpvcommons11n)
