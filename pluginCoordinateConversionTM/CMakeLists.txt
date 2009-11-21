MPV_PLUGIN_INIT(PluginCoordinateConversionTM)

SET(PluginCoordinateConversionTM_PRIVATE_HDRS
	CoordinateConverterEasyTM.h
	CoordinateConverterTM.h
	PluginCoordinateConversionTM.h
)
SET(PluginCoordinateConversionTM_SRCS
	CoordinateConverterEasyTM.cpp
	CoordinateConverterTM.cpp
	PluginCoordinateConversionTM.cpp
)

ADD_LIBRARY(PluginCoordinateConversionTM MODULE
    ${PluginCoordinateConversionTM_PUBLIC_HDRS}
    ${PluginCoordinateConversionTM_PRIVATE_HDRS}
    ${PluginCoordinateConversionTM_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginCoordinateConversionTM)

TARGET_LINK_LIBRARIES(PluginCoordinateConversionTM mpvcommon)
