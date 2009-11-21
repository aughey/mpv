MPV_PLUGIN_INIT(PluginCoordinateConversionGCC)

SET(PluginCoordinateConversionGCC_PRIVATE_HDRS
	CoordinateConverterGCC.h
	PluginCoordinateConversionGCC.h
)
SET(PluginCoordinateConversionGCC_SRCS
	CoordinateConverterGCC.cpp
	PluginCoordinateConversionGCC.cpp
)

ADD_LIBRARY(PluginCoordinateConversionGCC MODULE
    ${PluginCoordinateConversionGCC_PUBLIC_HDRS}
    ${PluginCoordinateConversionGCC_PRIVATE_HDRS}
    ${PluginCoordinateConversionGCC_SRCS})
MPV_PLUGIN_PROCESS_TARGET(PluginCoordinateConversionGCC)

TARGET_LINK_LIBRARIES(PluginCoordinateConversionGCC mpvcommon)
